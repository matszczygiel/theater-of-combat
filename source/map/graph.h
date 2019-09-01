#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <set>

#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>

class BidirectionalGraph {
   public:
    BidirectionalGraph& insert_node(int node, const std::set<int>& neighbors);

    BidirectionalGraph& remove_node(int node);
    BidirectionalGraph& remove_edge(int node1, int node2);

    const std::map<int, std::set<int>>& adjacency_matrix() const;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    std::map<int, std::set<int>> _adjacency_matrix{};
};

bool operator==(const BidirectionalGraph& lhs, const BidirectionalGraph& rhs);
bool operator!=(const BidirectionalGraph& lhs, const BidirectionalGraph& rhs);

template <class Archive>
void BidirectionalGraph::serialize(Archive& archive) {
    archive( CEREAL_NVP(_adjacency_matrix));
}

class WeightedBidirectionalGraph {
   public:
    WeightedBidirectionalGraph() = default;
    WeightedBidirectionalGraph(const BidirectionalGraph& graph,
                               int default_weight = 1);

    WeightedBidirectionalGraph& insert_edge(int node);

    WeightedBidirectionalGraph& insert_edge(int node1, int node2,
                                            int weight_1to2, int weight_2to1);

    WeightedBidirectionalGraph& change_edge_weight(int node1, int node2,
                                                   int weight_1to2);

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