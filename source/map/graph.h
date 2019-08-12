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

#endif