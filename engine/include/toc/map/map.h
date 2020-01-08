#ifndef MAP_H
#define MAP_H

#include <map>
#include <optional>
#include <set>
#include <tuple>

#include <cereal/types/map.hpp>

#include "graph.h"
#include "toc/core/id_gen.h"
#include "types.h"

class Map {
   public:
    enum class SiteType {
        hex,
        border,
    };
    using SiteId   = int;
    using SiteType = std::pair<SiteId, int>;

    const std::map<SiteId, HexSite>& hexes() const noexcept;
    const std::map<SiteId, BorderSite>& borders() const noexcept;
    const UnidirectionalGraph<std::pair<SiteId, int>>& graph() const noexcept;

    Map& insert(HexSite site);
    Map& insert(BorderSite site);

    SiteType type_of(SiteId id) const;

    std::optional<SiteId> get_hex_id(HexCoordinate coord) const noexcept;
    std::optional<HexCoordinate> get_hex_coord(SiteId id) const noexcept;

    std::set<SiteId> get_controlable_hexes_from(SiteId id) const;

    template <class Archive>
    void serialize(Archive& archive);

    static Map create_test_map();
    static Map create_plain_map(int width, int height);

   private:
    std::map<SiteId, HexSite> _hexes{};
    std::map<SiteId, BorderSite> _borders{};

    UnidirectionalGraph<SiteType> _graph{};

    IdGenerator<SiteId> _id_gen{0};
};

template <class Archive>
void Map::serialize(Archive& archive) {
    archive(CEREAL_NVP(_id_gen), CEREAL_NVP(_graph), CEREAL_NVP(_hexes),
            CEREAL_NVP(_borders));
}

#endif