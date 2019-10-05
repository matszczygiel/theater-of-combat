#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <queue>
#include <set>
#include <type_traits>

#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>

#include "core/log.h"

template <typename Index = int>
class BidirectionalGraph {
    static_assert(std::is_integral_v<Index>,
                  "BidirectionalGraph indexing type should be integral");

   public:
    BidirectionalGraph<Index>& insert_node(Index node, const std::set<Index>& neighbors);
    BidirectionalGraph<Index>& insert_node(Index node);
    BidirectionalGraph<Index>& insert_edge(Index node1, Index node2);
    BidirectionalGraph<Index>& remove_node(Index node);
    BidirectionalGraph<Index>& remove_edge(Index node1, Index node2);

    const std::map<Index, std::set<Index>>& adjacency_matrix() const noexcept;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    std::map<Index, std::set<Index>> _adjacency_matrix{};
};

template <typename Index>
BidirectionalGraph<Index>& BidirectionalGraph<Index>::insert_node(
    Index node, const std::set<Index>& neighbors) {
    for (const auto& n : neighbors) {
        insert_edge(node, n);
    }
    return *this;
}

template <typename Index>
BidirectionalGraph<Index>& BidirectionalGraph<Index>::insert_node(Index node) {
    _adjacency_matrix.insert({node, {}});
    return *this;
}

template <typename Index>
BidirectionalGraph<Index>& BidirectionalGraph<Index>::insert_edge(Index node1,
                                                                  Index node2) {
    _adjacency_matrix[node1].insert(node2);
    _adjacency_matrix[node2].insert(node1);
    return *this;
}

template <typename Index>
BidirectionalGraph<Index>& BidirectionalGraph<Index>::remove_node(Index node) {
    _adjacency_matrix.erase(node);

    for (auto& [_, n] : _adjacency_matrix)
        n.erase(node);

    return *this;
}

template <typename Index>
BidirectionalGraph<Index>& BidirectionalGraph<Index>::remove_edge(Index node1,
                                                                  Index node2) {
    if (_adjacency_matrix.count(node1) == 1 && _adjacency_matrix.count(node2) == 1) {
        _adjacency_matrix[node1].erase(node2);
        _adjacency_matrix[node2].erase(node1);
    }
    return *this;
}

template <typename Index>
const std::map<Index, std::set<Index>>& BidirectionalGraph<Index>::adjacency_matrix()
    const noexcept {
    return _adjacency_matrix;
}

template <typename Index>
bool operator==(const BidirectionalGraph<Index>& lhs,
                const BidirectionalGraph<Index>& rhs) {
    return lhs.adjacency_matrix() == rhs.adjacency_matrix();
}

template <typename Index>
bool operator!=(const BidirectionalGraph<Index>& lhs,
                const BidirectionalGraph<Index>& rhs) {
    return !(lhs == rhs);
}

template <typename Index>
template <class Archive>
void BidirectionalGraph<Index>::serialize(Archive& archive) {
    archive(CEREAL_NVP(_adjacency_matrix));
}

template <typename Index = int, class Weight = int>
class WeightedBidirectionalGraph {
    static_assert(std::is_integral_v<Index>,
                  "WeightedBidirectionalGraph indexing type should be integral");
    static_assert(std::is_arithmetic_v<Weight>,
                  "WeightedBidirectionalGraph weight type should be arithmetic");

   public:
    WeightedBidirectionalGraph() = default;
    WeightedBidirectionalGraph(const BidirectionalGraph<Index>& graph,
                               Weight default_weight = 1);

    WeightedBidirectionalGraph& insert_node(Index node);
    WeightedBidirectionalGraph& insert_edge(Index node1, Index node2, Weight weight_1to2,
                                            Weight weight_2to1);
    WeightedBidirectionalGraph& change_edge_weight(Index node1, Index node2,
                                                   Weight weight_1to2);
    WeightedBidirectionalGraph& remove_node(Index node);
    WeightedBidirectionalGraph& remove_edge(Index node1, Index node2);

    Weight weight(Index node_start, Index node_end) const;

    const std::map<Index, std::map<Index, Weight>>& adjacency_matrix() const noexcept;

    /// < < node, distance from src >, < node, previous node > >
    std::pair<std::map<Index, Weight>, std::map<Index, Index>> dijkstra(Index src) const;

   private:
    /// < node id, < neighbor, edge weight > >
    std::map<Index, std::map<Index, Weight>> _adjacency_matrix{};
};

template <typename Index, class Weight>
WeightedBidirectionalGraph<Index, Weight>::WeightedBidirectionalGraph(
    const BidirectionalGraph<Index>& graph, Weight default_weight) {
    for (const auto& [node, neighbors] : graph.adjacency_matrix()) {
        _adjacency_matrix.insert({node, {}});
        for (const auto& neighbor : neighbors)
            _adjacency_matrix[node].insert({neighbor, default_weight});
    }
}

template <typename Index, class Weight>
WeightedBidirectionalGraph<Index, Weight>&
WeightedBidirectionalGraph<Index, Weight>::insert_node(Index node) {
    _adjacency_matrix.insert({node, {}});
    return *this;
}

template <typename Index, class Weight>
WeightedBidirectionalGraph<Index, Weight>&
WeightedBidirectionalGraph<Index, Weight>::insert_edge(Index node1, Index node2,
                                                       Weight weight_1to2,
                                                       Weight weight_2to1) {
    _adjacency_matrix[node1].insert_or_assign(node2, weight_1to2);
    _adjacency_matrix[node2].insert_or_assign(node1, weight_2to1);
    return *this;
}

template <typename Index, class Weight>
WeightedBidirectionalGraph<Index, Weight>&
WeightedBidirectionalGraph<Index, Weight>::remove_node(Index node) {
    _adjacency_matrix.erase(node);
    for (auto& [_, n] : _adjacency_matrix)
        n.erase(node);

    return *this;
}

template <typename Index, class Weight>
WeightedBidirectionalGraph<Index, Weight>&
WeightedBidirectionalGraph<Index, Weight>::remove_edge(Index node1, Index node2) {
    if (_adjacency_matrix.count(node1) == 1 && _adjacency_matrix.count(node2) == 1) {
        _adjacency_matrix[node1].erase(node2);
        _adjacency_matrix[node2].erase(node1);
    }
    return *this;
}

template <typename Index, class Weight>
WeightedBidirectionalGraph<Index, Weight>&
WeightedBidirectionalGraph<Index, Weight>::change_edge_weight(Index node1, Index node2,
                                                              Weight weight_1to2) {
    engine_assert_throw(_adjacency_matrix.count(node1) == 1,
                        "Changing weight of nonexistent node {}.", node1);
    engine_assert_throw(_adjacency_matrix[node1].count(node2) == 1,
                        "Changing weight of nonexistent edge {} -> {}.", node1, node2);

    _adjacency_matrix[node1][node2] = weight_1to2;
    return *this;
}

template <typename Index, class Weight>
Weight WeightedBidirectionalGraph<Index, Weight>::weight(Index node_start,
                                                         Index node_end) const {
    return _adjacency_matrix.at(node_start).at(node_end);
}

template <typename Index, class Weight>
const std::map<Index, std::map<Index, Weight>>&
WeightedBidirectionalGraph<Index, Weight>::adjacency_matrix() const noexcept {
    return _adjacency_matrix;
}

template <typename Index, class Weight>
std::pair<std::map<Index, Weight>, std::map<Index, Index>>
WeightedBidirectionalGraph<Index, Weight>::dijkstra(Index src) const {
    constexpr auto infinity = std::numeric_limits<Weight>::max();

    std::map<Index, Weight> dist{};
    for (const auto& [node, _] : _adjacency_matrix) {
        dist[node] = infinity;
    }
    dist[src] = 0;

    std::priority_queue<std::pair<Weight, Index>, std::vector<std::pair<Weight, Index>>,
                        std::greater<std::pair<Weight, Index>>>
        queue;
    queue.push(std::make_pair(dist[src], src));

    std::map<Index, Index> prev;
    while (!queue.empty()) {
        auto u = queue.top().second;
        queue.pop();
        for (const auto& [vi, vw] : _adjacency_matrix.at(u)) {
            const auto alt = dist[u] + vw;
            if (alt < dist[vi]) {
                dist[vi] = alt;
                prev[vi] = u;
                queue.push(std::make_pair(dist[vi], vi));
            }
        }
    }

    return std::make_pair(dist, prev);
}

template <typename Index, class Weight>
bool operator==(const WeightedBidirectionalGraph<Index, Weight>& lhs,
                const WeightedBidirectionalGraph<Index, Weight>& rhs) {
    return lhs.adjacency_matrix() == rhs.adjacency_matrix();
}

template <typename Index, class Weight>
bool operator!=(const WeightedBidirectionalGraph<Index, Weight>& lhs,
                const WeightedBidirectionalGraph<Index, Weight>& rhs) {
    return !(lhs == rhs);
}

#endif