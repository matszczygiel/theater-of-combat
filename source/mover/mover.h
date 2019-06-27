#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>

#include "map/hex_site.h"
#include "map/passage_site.h"

class Message_bus;
class Map;
class Unit;

class Mover {
   public:
    explicit Mover(Unit* unit, Map& map);

    void find_paths();
    void move(const sf::Vector2f& mouse_pos, const std::set<int>& forbidden_sites,
              std::shared_ptr<Message_bus>& bus);

    virtual ~Mover() = default;

   protected:
    virtual std::unordered_map<Hex_site::Type, int> create_hex_table() const      = 0;
    virtual std::unordered_map<Passage_site::Type, int> create_pass_table() const = 0;

    std::map<int, int> compute_weights(
        const std::vector<Hex_site>& hex_set,
        const std::map<int, std::unique_ptr<Passage_site>>& pass_set) const;
    void clear();

    std::vector<std::pair<int, int>> find_path(const sf::Vector2f& mouse_pos);

    Map* _map;
    Unit* _unit;

    std::map<int, int> _distances;
    std::map<int, int> _previous;
};

std::pair<std::map<int, int>, std::map<int, int>>
dijkstra(const std::map<int, std::vector<int>>& graph, const int& src,
         const std::map<int, int>& weights);
