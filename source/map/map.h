#ifndef MAP_H
#define MAP_H

#include <map>

#include "graph.h"
#include "types.h"

class Map {
   public:
    const std::map<int, HexSite>& hexes() const;
    const std::map<int, RiverSite>& rivers() const;
    const BidirectionalGraph& graph() const;

    void insert(HexSite site);
    void insert(RiverSite site);

    static Map create_test_map();

   private:
    int fetch_id();

    std::map<int, HexSite> _hexes{};
    std::map<int, RiverSite> _rivers{};

    BidirectionalGraph _graph{};

    int _current_free_id{0};
};

#endif