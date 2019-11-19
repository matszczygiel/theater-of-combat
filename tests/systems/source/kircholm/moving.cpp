#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "tsys/kircholm.h"

template <class T, class U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& value) {
    os << "( " << value.first << ", " << value.second << ')';
    return os;
}

template <class T, class W>
std::ostream& operator<<(std::ostream& os,
                         const WeightedBidirectionalGraph<T, W>& value) {
    os << '{';
    for (const auto& [node, neighbors] : value.adjacency_matrix()) {
        os << "{ " << node << ", {";
        for (const auto& [n, w] : neighbors)
            os << " [" << n << " -> " << w << " ]";
        os << " }}\n";
    }
    os << '}';
    return os;
}

using namespace kirch;

static Map simple_map() {
    Map res;
    res.insert(HexSite(HexCoordinate(-1, -1), static_cast<int>(HexType::field)));
    res.insert(HexSite(HexCoordinate(0, -1), static_cast<int>(HexType::forest)));
    res.insert(HexSite(HexCoordinate(1, -1), static_cast<int>(HexType::forest)));
    res.insert(HexSite(HexCoordinate(-2, 0), static_cast<int>(HexType::field)));
    res.insert(HexSite(HexCoordinate(-1, 0), static_cast<int>(HexType::field)));
    res.insert(HexSite(HexCoordinate(0, 0), static_cast<int>(HexType::forest)));
    res.insert(HexSite(HexCoordinate(1, 0), static_cast<int>(HexType::forest)));
    res.insert(HexSite(HexCoordinate(-2, 1), static_cast<int>(HexType::field)));
    res.insert(HexSite(HexCoordinate(-1, 1), static_cast<int>(HexType::field)));
    res.insert(HexSite(HexCoordinate(0, 1), static_cast<int>(HexType::field)));
    return res;
}

static UnitManager simple_unit_manager() {
    UnitManager um;
    const auto u0  = um.create(static_cast<int>(UnitType::infrantry), "test unit 0");
    const auto& mc = um.assign_component<MovementComponent>(u0, 10);
    auto& pc       = *um.get_component<PositionComponent>(u0);
    pc.position    = HexCoordinate(1, -1);
    pc.direction   = 1;
    return um;
}

static std::shared_ptr<Scenario> simple_scenario() {
    auto sc                  = std::make_shared<Scenario>();
    sc->map                  = simple_map();
    sc->units                = simple_unit_manager();
    sc->teams["test team 0"] = {0};
    sc->teams["test team 1"] = {};
    return sc;
}

TEST_CASE("creating graph") {
    const auto sc = simple_scenario();
    kirch::MovementSystem mov(sc);

    SUBCASE("hexes") {
        const auto wg = mov.make_weighted_graph(0);

        WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, kirch::Movability> ref(
            sc->map.graph(), 1);

        ref.change_edge_weight({0, 0}, {1, 0}, 2)
            .change_edge_weight({1, 0}, {2, 0}, 2)
            .change_edge_weight({1, 5}, {5, 5}, 2)
            .change_edge_weight({2, 3}, {1, 3}, 2)
            .change_edge_weight({2, 4}, {5, 4}, 2)
            .change_edge_weight({2, 5}, {6, 5}, 2)
            .change_edge_weight({4, 0}, {5, 0}, 2)
            .change_edge_weight({4, 1}, {1, 1}, 2)
            .change_edge_weight({5, 0}, {6, 0}, 2)
            .change_edge_weight({5, 1}, {2, 1}, 2)
            .change_edge_weight({5, 2}, {1, 2}, 2)
            .change_edge_weight({6, 2}, {2, 2}, 2)
            .change_edge_weight({6, 3}, {5, 3}, 2)
            .change_edge_weight({8, 1}, {5, 1}, 2)
            .change_edge_weight({9, 1}, {6, 1}, 2)
            .change_edge_weight({9, 2}, {5, 2}, 2);

        CHECK_EQ(wg, ref);
    }

    SUBCASE("path finding") {
        CHECK(mov.init_movement(HexCoordinate(1, -1), {"test team 0"}, {"test team 1"}));
        CHECK(mov.is_moving());
        mov.reset();
        CHECK_FALSE(mov.is_moving());
        CHECK(mov.init_movement(HexCoordinate(1, -1), {"test team 0"}, {"test team 1"}));
        CHECK(mov.is_moving());

        const auto prev1 = mov.path_preview(HexCoordinate(1, -1), 3);
        std::vector<std::tuple<HexCoordinate, int, Movability>> ref1 = {
            {HexCoordinate(1, -1), 1, 0},
            {HexCoordinate(1, -1), 2, 1},
            {HexCoordinate(1, -1), 3, 2}};

        CHECK_EQ(prev1, ref1);

        const auto prev2             = mov.path_preview(HexCoordinate(-1, 0), 2);
        const auto& [hex, dir, cost] = prev2.back();
        CHECK_EQ(hex, HexCoordinate(-1, 0));
        CHECK_EQ(dir, 2);
        CHECK_EQ(cost, 8);

        for (int i = 1; i < prev2.size(); ++i) {
            const auto& [hex0, dir0, cost0] = prev2.at(i - 1);
            const auto& [hex1, dir1, cost1] = prev2.at(i);
            auto diff                       = std::abs(dir0 - dir1);
            diff                            = (diff < 3) ? diff : 6 - diff;
            const auto cost                 = cost1 - cost0;
            if (hex0 == hex1) {
                CHECK_EQ(diff, 1);
                CHECK_EQ(cost, 1);
            } else {
                const auto neighbors = hex0.neighbors();
                CHECK_EQ(std::count(neighbors.cbegin(), neighbors.cend(), hex1), 1);
                CHECK_EQ(dir0, dir1);
                CHECK_NE(cost, 0);
            }
        }
    }
}