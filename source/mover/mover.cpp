#include "mover.h"

#include <limits>

#include <SFML/System/Vector2.hpp>

#include "log.h"

Mover::Mover(std::weak_ptr<Unit> unit, std::weak_ptr<Map> map)
    : _map(map), _unit(unit) {}

std::map<int, int> Mover::compute_weights(
    const std::vector<std::unique_ptr<Hex_site>>& hex_set,
    const std::map<int, std::unique_ptr<Passage_site>>& pass_set) const {
    std::map<int, int> weights;
    const auto table = create_table();

    for (const auto& x : hex_set) {
        const auto& no   = x->get_number();
        const auto& name = x->get_name();
        weights[no]      = table.at(name);
    }
    for (const auto& x : pass_set) {
        const auto& no   = x.first;
        const auto& name = x.second->get_name();
        weights[no]      = table.at(name);
    }

    return weights;
}

void Mover::find_paths() {
    GAME_INFO("Initializing path finding algorithm.");
    const auto map  = _map.lock();
    const auto unit = _unit.lock();

    if (map && unit) {
        const auto src     = unit->get_ocupation()->get_number();
        const auto weights = compute_weights(map->_hexes, map->_passages);
        auto res           = dijkstra(map->_adjacency_matrix, src, weights);
        auto& dist         = res.first;
        auto& prev         = res.second;
        const auto& mv_pts = unit->get_mv_points();

        for (auto it = dist.begin(); it != dist.end();)
            if (it->second > mv_pts) {
                prev.erase(it->first);
                it = dist.erase(it);
            } else {
                if (map->_hexes.size() > it->first) {
                    map->get_hex(it->first)->set_highlighted(true);
                } else {
                    map->get_pass(it->first)->set_highlighted(true);
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

void Mover::move(const sf::Vector2f& mouse_pos) {
    GAME_INFO("Moving unit.");
    const auto map  = _map.lock();
    const auto unit = _unit.lock();

    if (map && unit) {
        for (auto it = _distances.begin(); it != _distances.end(); ++it) {
            if (map->_hexes.size() > it->first) {
                const auto& hex = map->get_hex(it->first);

                if (hex->contains(mouse_pos)) {
                    unit->place_on_hex(hex);
                    unit->reduce_mv_points(it->second);

                    clear();
                    return;
                }
            }
        }
    }

    clear();
    return;
}

void Mover::clear() {
    _distances.empty();
    _previous.empty();
    if (auto map = _map.lock()) {
        for (auto& x : map->_hexes)
            x->set_highlighted(false);
        for (auto& x : map->_passages)
            x.second->set_highlighted(false);
    }
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