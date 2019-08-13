#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <set>

class BidirectionalGraph {
   public:
    BidirectionalGraph& insert_node(int node, const std::set<int>& neighbors);

    BidirectionalGraph& remove_node(int node);

    const std::map<int, std::set<int>>& adjacency_matrix() const;

   private:
    std::map<int, std::set<int>> _adjacency_matrix{};
};

bool operator==(const BidirectionalGraph& lhs, const BidirectionalGraph& rhs);
bool operator!=(const BidirectionalGraph& lhs, const BidirectionalGraph& rhs);

class WeightedBidirectionalGraph {
   public:
    WeightedBidirectionalGraph& insert_edge(int node1, int node2,
                                            int weight_1to2, int weight_2to1);

    WeightedBidirectionalGraph& remove_edge(int node1, int node2);

    const std::map<int, std::set<std::pair<int, int>>>& adjacency_matrix()
        const;

   private:
    // < node id, < neighbor, edge weight > >
    std::map<int, std::set<std::pair<int, int>>> _adjacency_matrix{};
};

#endif