#pragma once

#include <map>
#include <memory>
#include <unordered_map>

#include "map/map.h"
#include "unit/unit.h"

class Mover {
   public:
    explicit Mover(std::weak_ptr<Unit> unit, std::weak_ptr<Map> map);

    void find_paths();
    void move(const sf::Vector2f& mouse_pos);

    virtual ~Mover() = default;

   protected:
    virtual std::unordered_map<Map_site::id_type, int> create_table() const = 0;

    std::map<int, int> compute_weights(
        const std::vector<std::unique_ptr<Hex_site>>& hex_set,
        const std::map<int, std::unique_ptr<Passage_site>>& pass_set) const;
    void clear();

    std::weak_ptr<Map> _map;
    std::weak_ptr<Unit> _unit;

    std::map<int, int> _distances;
    std::map<int, int> _previous;
};

std::pair<std::map<int, int>, std::map<int, int>>
dijkstra(const std::map<int, std::vector<int>>& graph, const int& src,
         const std::map<int, int>& weights);