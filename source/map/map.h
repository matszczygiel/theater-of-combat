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

    constexpr const std::map<SiteId, HexSite>& hexes() const noexcept;
    constexpr const std::map<SiteId, RiverSite>& rivers() const noexcept;
    constexpr const BidirectionalGraph<SiteId>& graph() const noexcept;

    Map& insert(HexSite site);
    Map& insert(RiverSite site);

    SiteType type_of(SiteId id) const;

    std::optional<SiteId> get_hex_id(HexCoordinate coord) const noexcept;
    std::optional<HexCoordinate> get_hex_coord(SiteId id) const noexcept;

    std::set<SiteId> get_controlable_hexes_from(SiteId id) const;

    template <class Archive>
    void serialize(Archive& archive);

    static Map create_test_map();

   private:
    constexpr SiteId fetch_id() noexcept;

    std::map<SiteId, HexSite> _hexes{};
    std::map<SiteId, RiverSite> _rivers{};

    BidirectionalGraph<SiteId> _graph{};
    SiteId _current_free_id{0};
};

template <class Archive>
void Map::serialize(Archive& archive) {
    archive(CEREAL_NVP(_current_free_id), CEREAL_NVP(_graph), CEREAL_NVP(_hexes),
            CEREAL_NVP(_rivers));
}

#endif