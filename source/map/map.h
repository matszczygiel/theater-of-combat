#ifndef MAP_H
#define MAP_H

#include <map>

#include <cereal/types/map.hpp>

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

    template <class Archive>
    void serialize(Archive& archive);

   private:
    int fetch_id();

    std::map<int, HexSite> _hexes{};
    std::map<int, RiverSite> _rivers{};

    BidirectionalGraph _graph{};

    int _current_free_id{0};
};

template <class Archive>
void Map::serialize(Archive& archive) {
    archive(CEREAL_NVP(_current_free_id), CEREAL_NVP(_graph),
            CEREAL_NVP(_hexes), CEREAL_NVP(_rivers));
}

#endif