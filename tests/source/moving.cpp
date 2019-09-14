#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../../source/moving/mover.h"

static Map simple_test_map() {
    Map res;
    res.insert(HexSite(HexCoordinate(-1, -1), HexType::field));
    res.insert(HexSite(HexCoordinate(0, -1), HexType::forest));
    res.insert(HexSite(HexCoordinate(1, -1), HexType::forest));
    res.insert(HexSite(HexCoordinate(-2, 0), HexType::field));
    res.insert(HexSite(HexCoordinate(-1, 0), HexType::field));
    res.insert(HexSite(HexCoordinate(0, 0), HexType::forest));
    res.insert(HexSite(HexCoordinate(1, 0), HexType::forest));
    res.insert(HexSite(HexCoordinate(-2, 1), HexType::field));
    res.insert(HexSite(HexCoordinate(-1, 1), HexType::field));
    res.insert(HexSite(HexCoordinate(0, 1), HexType::field));
    return res;
}

TEST_CASE("creating graph") {
    SUBCASE("only hexes") {
        const Map map             = simple_test_map();
        const auto type           = UnitType::heavy;
        const auto weighted_graph = mover::make_weighted_graph(map, type);
        WeightedBidirectionalGraph ref;
        ref.insert_edge(0, 1, 6, 2)
            .insert_edge(0, 3, 2, 2)
            .insert_edge(0, 4, 2, 2)
            .insert_edge(1, 2, 6, 6)
            .insert_edge(1, 4, 2, 6)
            .insert_edge(1, 5, 6, 6)
            .insert_edge(2, 5, 6, 6)
            .insert_edge(2, 6, 6, 6)
            .insert_edge(3, 4, 2, 2)
            .insert_edge(3, 7, 2, 2)
            .insert_edge(4, 5, 6, 2)
            .insert_edge(4, 7, 2, 2)
            .insert_edge(4, 8, 2, 2)
            .insert_edge(5, 6, 6, 6)
            .insert_edge(5, 8, 2, 6)
            .insert_edge(5, 9, 2, 6)
            .insert_edge(6, 9, 2, 6)
            .insert_edge(7, 8, 2, 2)
            .insert_edge(8, 9, 2, 2);

        CHECK_EQ(weighted_graph, ref);
    }

    SUBCASE("rivers") {
        Map map = simple_test_map();
        map.insert(RiverSite(HexCoordinate(-1, -1), HexCoordinate(-2, 0), RiverType::small));
        map.insert(RiverSite(HexCoordinate(-1, 0), HexCoordinate(-2, 0), RiverType::small));
        map.insert(RiverSite(HexCoordinate(-1, 0), HexCoordinate(-2, 1), RiverType::stream));
        map.insert(RiverSite(HexCoordinate(-1, 0), HexCoordinate(-1, 1), RiverType::stream));
        map.insert(RiverSite(HexCoordinate(0, 0), HexCoordinate(-1, 1), RiverType::stream));
        map.insert(RiverSite(HexCoordinate(-1, 1), HexCoordinate(0, 1), RiverType::stream));

        const auto type           = UnitType::heavy;
        const auto weighted_graph = mover::make_weighted_graph(map, type);

        WeightedBidirectionalGraph ref;
        ref.insert_edge(0, 1, 6, 2)
            .insert_edge(0, 3, 5, 5)
            .insert_edge(0, 4, 2, 2)
            .insert_edge(1, 2, 6, 6)
            .insert_edge(1, 4, 2, 6)
            .insert_edge(1, 5, 6, 6)
            .insert_edge(2, 5, 6, 6)
            .insert_edge(2, 6, 6, 6)
            .insert_edge(3, 4, 5, 5)
            .insert_edge(3, 7, 2, 2)
            .insert_edge(4, 5, 6, 2)
            .insert_edge(4, 7, 6, 6)
            .insert_edge(4, 8, 6, 6)
            .insert_edge(5, 6, 6, 6)
            .insert_edge(5, 8, 6, 10)
            .insert_edge(5, 9, 2, 6)
            .insert_edge(6, 9, 2, 6)
            .insert_edge(7, 8, 2, 2)
            .insert_edge(8, 9, 6, 6);

        CHECK_EQ(weighted_graph, ref);
    }
}