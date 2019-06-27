#include "mover.h"

#include <limits>

#include <SFML/System/Vector2.hpp>

#include "log.h"
#include "map/map.h"
#include "messaging/concrete_message.h"
#include "unit/unit.h"

Mover::Mover(Unit* unit, Map& map)
    : _map{&map}, _unit{unit} {}

std::map<int, int> Mover::compute_weights(
    const std::vector<Hex_site>& hex_set,
    const std::map<int, std::unique_ptr<Passage_site>>& pass_set) const {
    std::map<int, int> weights;
    const auto hex_table  = create_hex_table();
    const auto pass_table = create_pass_table();

    for (const auto& x : hex_set) {
        const auto& no   = x.get_number();
        const auto& type = x.get_type();
        weights[no]      = hex_table.at(type);
    }
    for (const auto& x : pass_set) {
        const auto& no   = x.first;
        const auto& type = x.second->get_type();
        weights[no]      = pass_table.at(type);
    }

    return weights;
}

#include "messaging/concrete_message.h"

void Mover::find_paths() {
    GAME_INFO("Initializing path finding algorithm.");
    if (_map && _unit) {
        const auto src     = _unit->get_occupation()->get_number();
        const auto weights = compute_weights(_map->_hexes, _map->_passages);
        auto res           = dijkstra(_map->_adjacency_matrix, src, weights);
        auto& dist         = res.first;
        auto& prev         = res.second;
        const auto& mv_pts = _unit->get_mv_points();

        for (auto it = dist.begin(); it != dist.end();)
            if (it->second > mv_pts) {
                prev.erase(it->first);
                it = dist.erase(it);
            } else {
                if (static_cast<int>(_map->_hexes.size()) > it->first) {
                    _map->get_hex(it->first).set_highlighted(true);
                } else {
                    _map->get_pass(it->first).set_highlighted(true);
                }

                ++it;
            }

        _distances = dist;
        _previous  = prev;

    } else {
        ENGINE_ERROR("Initializing mover with invalid map or unit.");
        assert(false);
    }
}

void Mover::move(const sf::Vector2f& mouse_pos, const std::set<int>& forbidden_sites,
                 std::shared_ptr<Message_bus>& bus) {
    ENGINE_INFO("Moving unit.");
    const auto path = find_path(mouse_pos);
    if (path.empty())
        return;

    auto dest = path.back();

    for (auto& site : path) {
        if (forbidden_sites.count(site.first)) {
            dest = site;
            break;
        }
    }
/*
    _unit->place_on_hex(&_map->get_hex(dest.first));
    _unit->reduce_mv_points(dest.second);
*/
    bus->queue_message(std::make_shared<Unit_moved_msg>(_unit->get_id(), dest.first, dest.second));
}

std::vector<std::pair<int, int>> Mover::find_path(const sf::Vector2f& mouse_pos) {
    ENGINE_ASSERT(_map && _unit, "Trying to move unit on map but one of them is invalid.");

    const auto& hsize = static_cast<int>(_map->_hexes.size());
    std::vector<std::pair<int, int>> vec;

    for (auto it = _distances.begin(); it != _distances.end(); ++it) {
        if (hsize > it->first) {
            auto& hex = _map->get_hex(it->first);

            if (hex.contains(mouse_pos)) {
                vec.emplace_back(*it);
                while (true) {
                    auto search = _previous.find(vec.back().first);
                    if (search == _previous.end())
                        break;

                    const auto& no = search->second;
                    vec.emplace_back(no, _distances[no]);
                }
                for (auto v = vec.begin(); v != vec.end();) {
                    if (v->first >= hsize) {
                        v = vec.erase(v);
                    } else {
                        ++v;
                    }
                }
                break;
            }
        }
    }

    clear();
    std::reverse(std::begin(vec), std::end(vec));
    return vec;
}

void Mover::clear() {
    _distances.empty();
    _previous.empty();
    for (auto& x : _map->_hexes)
        x.set_highlighted(false);
    for (auto& x : _map->_passages)
        x.second->set_highlighted(false);
}

std::pair<std::map<int, int>, std::map<int, int>>
dijkstra(const std::map<int, std::vector<int>>& graph, const int& src,
         const std::map<int, int>& weights) {
    std::map<int, int> dist;

    constexpr auto infinity = std::numeric_limits<int>::max();

    for (auto it = graph.begin(); it != graph.end(); ++it) {
        dist[it->first] = infinity;
    }
    dist[src] = 0;

    std::map<int, int> prev;
    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>
        queue;

    queue.push(std::make_pair(dist[src], src));

    while (!queue.empty()) {
        auto u = queue.top().second;
        queue.pop();
        for (const auto& v : graph.at(u)) {
            const auto alt = dist[u] + weights.at(v);
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                queue.push(std::make_pair(dist[v], v));
            }
        }
    }

    return std::make_pair(dist, prev);
}
