#include "graph.h"

#include <cassert>

BidirectionalGraph& BidirectionalGraph::insert_node(
    int node, const std::set<int>& neighbors) {
    for (const auto& n : neighbors) {
        auto search = _adjacency_matrix.find(n);
        if (_adjacency_matrix.count(n) == 1) {
            assert(_adjacency_matrix[n].insert(node).second);
        } else {
            throw std::runtime_error("Neighbors not present in the graph.");
        }
    }
    if (!_adjacency_matrix.insert({node, neighbors}).second)
        throw std::runtime_error("Node already present in the graph.");

    return *this;
}

BidirectionalGraph& BidirectionalGraph::remove_node(int node) {
    if (_adjacency_matrix.erase(node) != 1)
        throw std::runtime_error("Removing nonexistent node.");

    for (auto& n : _adjacency_matrix)
        n.second.erase(node);

    return *this;
}

const std::map<int, std::set<int>>& BidirectionalGraph::adjacency_matrix()
    const {
    return _adjacency_matrix;
}

bool operator==(const BidirectionalGraph& lhs, const BidirectionalGraph& rhs) {
    return lhs.adjacency_matrix() == rhs.adjacency_matrix();
}

bool operator!=(const BidirectionalGraph& lhs, const BidirectionalGraph& rhs) {
    return !(lhs == rhs);
}

WeightedBidirectionalGraph& WeightedBidirectionalGraph::insert_edge(
    int node1, int node2, int weight_1to2, int weight_2to1) {
    if (_adjacency_matrix.count(node1) == 1) {
        auto& neighbors = _adjacency_matrix[node1];
        for (auto& n : neighbors) {
            if (n.first == node2)
                throw std::runtime_error("Edge already inserted");
        }
        neighbors.insert({node2, weight_1to2});

    } else {
        _adjacency_matrix[node1] = {{node2, weight_1to2}};
    }

    /// TO DO
    for (const auto& n : neighbors) {
        auto search = _adjacency_matrix.find(n);
        if (_adjacency_matrix.count(n) == 1) {
            assert(_adjacency_matrix[n].insert(node).second);
        } else {
            throw std::runtime_error("Neighbors not present in the graph.");
        }
    }
    if (!_adjacency_matrix.insert({node, neighbors}).second)
        throw std::runtime_error("Node already present in the graph.");

    return *this;
}

WeightedBidirectionalGraph& WeightedBidirectionalGraph::remove_edge(int node1,
                                                                    int node2);