#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <set>
#include <type_traits>

#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>

#include "core/log.h"

template <typename T = int>
class BidirectionalGraph {
    static_assert(std::is_integral_v<T>,
                  "BidirectionalGraph indexing type should be integral");

   public:
    BidirectionalGraph<T>& insert_node(T node, const std::set<T>& neighbors);
    BidirectionalGraph<T>& insert_edge(T node1, T node2);
    BidirectionalGraph<T>& remove_node(T node);
    BidirectionalGraph<T>& remove_edge(T node1, T node2);

    const std::map<T, std::set<T>>& adjacency_matrix() const;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    std::map<T, std::set<T>> _adjacency_matrix{};
};

template <typename T>
BidirectionalGraph<T>& BidirectionalGraph<T>::insert_node(T node,
                                                          const std::set<T>& neighbors) {
    for (const auto& n : neighbors) {
        insert_edge(node, n);
    }

    return *this;
}

template <typename T>
BidirectionalGraph<T>& BidirectionalGraph<T>::insert_edge(T node1, T node2) {
    _adjacency_matrix[node1].insert(node2);
    _adjacency_matrix[node2].insert(node1);

    return *this;
}

template <typename T>
BidirectionalGraph<T>& BidirectionalGraph<T>::remove_node(T node) {
    _adjacency_matrix.erase(node);

    for (auto& n : _adjacency_matrix)
        n.second.erase(node);

    return *this;
}

template <typename T>
BidirectionalGraph<T>& BidirectionalGraph<T>::remove_edge(T node1, T node2) {
    if (_adjacency_matrix.count(node1) != 1 || _adjacency_matrix.count(node2) != 1)
        return *this;

    _adjacency_matrix[node1].erase(node2);
    _adjacency_matrix[node2].erase(node1);
    return *this;
}

template <typename T>
const std::map<T, std::set<T>>& BidirectionalGraph<T>::adjacency_matrix() const {
    return _adjacency_matrix;
}

template <typename T>
bool operator==(const BidirectionalGraph<T>& lhs, const BidirectionalGraph<T>& rhs) {
    return lhs.adjacency_matrix() == rhs.adjacency_matrix();
}

template <typename T>
bool operator!=(const BidirectionalGraph<T>& lhs, const BidirectionalGraph<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
template <class Archive>
void BidirectionalGraph<T>::serialize(Archive& archive) {
    archive(CEREAL_NVP(_adjacency_matrix));
}

template <typename T, class Weight>
class WeightedBidirectionalGraph {
    static_assert(std::is_integral_v<T>,
                  "WeightedBidirectionalGraph indexing type should be integral");
    static_assert(std::is_arithmetic_v<Weight>,
                  "WeightedBidirectionalGraph weight type should be arithmetic");

   public:
    WeightedBidirectionalGraph() = default;
    WeightedBidirectionalGraph(const BidirectionalGraph<T>& graph,
                               Weight default_weight = 1);

    WeightedBidirectionalGraph& insert_node(T node);
    WeightedBidirectionalGraph& insert_edge(T node1, T node2, Weight weight_1to2,
                                            Weight weight_2to1);
    WeightedBidirectionalGraph& change_edge_weight(int node1, int node2, int weight_1to2);
    int weight(int node_start, int node_end) const;

    WeightedBidirectionalGraph& remove_node(int node);

    const std::map<int, std::map<int, int>>& adjacency_matrix() const;

    /// < < node, distance from src >, < node, previous node > >
    std::pair<std::map<int, int>, std::map<int, int>> dijkstra(int src) const;

   private:
    void check_and_insert_half_of_edge(int node1, int node2, int weight_1to2);

    /// < node id, < neighbor, edge weight > >
    std::map<int, std::map<int, int>> _adjacency_matrix{};
};

bool operator==(const WeightedBidirectionalGraph& lhs,
                const WeightedBidirectionalGraph& rhs);
bool operator!=(const WeightedBidirectionalGraph& lhs,
                const WeightedBidirectionalGraph& rhs);

#endif