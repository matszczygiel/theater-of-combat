#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "tsys/kircholm.h"

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

        WeightedBidirectionalGraph<std::pair<Map::SiteId, int>, kirch::Movability> ref(
            sc->map.graph(), 1);

        ref.change_edge_weight({0, 0}, {1, 3}, 2)
            .change_edge_weight({4, 1}, {1, 4}, 2)
            .change_edge_weight({4, 0}, {5, 3}, 2)
            .change_edge_weight({8, 1}, {5, 4}, 2)
            .change_edge_weight({9, 2}, {5, 5}, 2)
            .change_edge_weight({9, 1}, {6, 4}, 2)
            .change_edge_weight({4, 1}, {1, 4}, 2)
            .change_edge_weight({1, 0}, {2, 3}, 2)
            .change_edge_weight({1, 5}, {5, 2}, 2)
            .change_edge_weight({5, 0}, {6, 3}, 2)
            .change_edge_weight({5, 1}, {2, 4}, 2)
            .change_edge_weight({5, 2}, {1, 5}, 2)
            .change_edge_weight({2, 3}, {1, 0}, 2)
            .change_edge_weight({2, 4}, {5, 1}, 2)
            .change_edge_weight({2, 5}, {6, 2}, 2)
            .change_edge_weight({6, 2}, {2, 5}, 2)
            .change_edge_weight({6, 3}, {5, 0}, 2);

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

        const auto prev2 = mov.path_preview(HexCoordinate(-1, 0), 2);
        const auto& [hex, dir, cost] = prev2.back();
        CHECK_EQ(hex, HexCoordinate(-1, 0));
        CHECK_EQ(dir, 2);
        CHECK_EQ(cost, 8);
    }
}