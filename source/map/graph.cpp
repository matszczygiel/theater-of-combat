#include "graph.h"

#include <cassert>
#include <limits>
#include <queue>

BidirectionalGraph& BidirectionalGraph::insert_node(
    int node, const std::set<int>& neighbors) {
    for (const auto& n : neighbors) {
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

BidirectionalGraph& BidirectionalGraph::remove_edge(int node1, int node2) {
    if (_adjacency_matrix.count(node1) != 1 ||
        _adjacency_matrix.count(node2) != 1)
        throw std::logic_error("Removing edge connecting nonexistent nodes.");

    if (_adjacency_matrix[node1].erase(node2) != 1)
        throw std::logic_error("Removing nonexistent edge.");

    if (_adjacency_matrix[node2].erase(node1) != 1)
        throw std::logic_error("Removing nonexistent edge.");

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

WeightedBidirectionalGraph::WeightedBidirectionalGraph(
    const BidirectionalGraph& graph, int default_weight) {
    for (const auto& [node, neighbors] : graph.adjacency_matrix()) {
        _adjacency_matrix[node] = {};
        for (const auto& neighbor : neighbors)
            _adjacency_matrix[node].insert({neighbor, default_weight});
    }
}

WeightedBidirectionalGraph& WeightedBidirectionalGraph::insert_edge(int node) {
    if (_adjacency_matrix.count(node) == 1)
        throw std::runtime_error("Node already present in the graph.");

    _adjacency_matrix[node] = {};

    return *this;
}

void WeightedBidirectionalGraph::check_and_insert_half_of_edge(
    int node1, int node2, int weight_1to2) {
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
}

WeightedBidirectionalGraph& WeightedBidirectionalGraph::insert_edge(
    int node1, int node2, int weight_1to2, int weight_2to1) {
    check_and_insert_half_of_edge(node1, node2, weight_1to2);
    check_and_insert_half_of_edge(node2, node1, weight_2to1);

    return *this;
}

WeightedBidirectionalGraph& WeightedBidirectionalGraph::remove_node(int node) {
    if (_adjacency_matrix.erase(node) != 1)
        throw std::runtime_error("Removing nonexistent node.");

    for (auto& n : _adjacency_matrix)
        n.second.erase(node);

    return *this;
}

WeightedBidirectionalGraph& WeightedBidirectionalGraph::change_edge_weight(
    int node1, int node2, int weight_1to2) {
    if (_adjacency_matrix.count(node1) != 1)
        throw std::runtime_error("Changing weight of nonexistent node.");

    if (_adjacency_matrix[node1].count(node2) != 1)
        throw std::runtime_error("Changing weight of nonexistent edge.");

    _adjacency_matrix[node1][node2] = weight_1to2;

    return *this;
}

std::pair<std::map<int, int>, std::map<int, int>>
WeightedBidirectionalGraph::dijkstra(int src) const {
    constexpr auto infinity = std::numeric_limits<int>::max();

    std::map<int, int> dist;
    for (const auto& node : _adjacency_matrix) {
        dist[node.first] = infinity;
    }
    dist[src] = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>
        queue;
    queue.push(std::make_pair(dist[src], src));

    std::map<int, int> prev;
    while (!queue.empty()) {
        auto u = queue.top().second;
        queue.pop();
        for (const auto& v : _adjacency_matrix.at(u)) {
            const auto alt = dist[u] + v.second;
            if (alt < dist[v.first]) {
                dist[v.first] = alt;
                prev[v.first] = u;
                queue.push(std::make_pair(dist[v.first], v.first));
            }
        }
    }

    return std::make_pair(dist, prev);
}

const std::map<int, std::map<int, int>>&
WeightedBidirectionalGraph::adjacency_matrix() const {
    return _adjacency_matrix;
}

bool operator==(const WeightedBidirectionalGraph& lhs,
                const WeightedBidirectionalGraph& rhs) {
    return lhs.adjacency_matrix() == rhs.adjacency_matrix();
}

bool operator!=(const WeightedBidirectionalGraph& lhs,
                const WeightedBidirectionalGraph& rhs) {
    return !(lhs == rhs);
}
