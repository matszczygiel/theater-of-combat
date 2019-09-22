#ifndef MAP_H
#define MAP_H

#include <map>
#include <optional>
#include <set>

#include <cereal/types/map.hpp>

#include "graph.h"
#include "types.h"

class Map {
   public:
    enum class SiteType {
        hex,
        river,
    };

    using SiteId = int;

    const std::map<SiteId, HexSite>& hexes() const;
    const std::map<SiteId, RiverSite>& rivers() const;
    const BidirectionalGraph& graph() const;

    void insert(HexSite site);
    void insert(RiverSite site);

    SiteType type_of(SiteId id) const;

    static Map create_test_map();

    std::optional<SiteId> get_hex_id(HexCoordinate coord) const;
    std::optional<HexCoordinate> get_hex_coord(SiteId id) const;

    std::set<SiteId> get_controlable_hexes_from(SiteId id) const;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    SiteId fetch_id();

    std::map<SiteId, HexSite> _hexes{};
    std::map<SiteId, RiverSite> _rivers{};

    BidirectionalGraph _graph{};

    SiteId _current_free_id{0};
};

template <class Archive>
void Map::serialize(Archive& archive) {
    archive(CEREAL_NVP(_current_free_id), CEREAL_NVP(_graph),
            CEREAL_NVP(_hexes), CEREAL_NVP(_rivers));
}

#endif