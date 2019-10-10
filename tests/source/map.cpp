#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "map/graph.h"
#include "map/hexagons.h"
#include "toc/map/map.h"
#include "map/types.h"

static Map simple_test_map() {
    Map res;
    // hexes
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

    // rivers
    res.insert(RiverSite(HexCoordinate(-1, -1), HexCoordinate(-2, 0), RiverType::small));
    res.insert(RiverSite(HexCoordinate(-1, 0), HexCoordinate(-2, 0), RiverType::small));
    res.insert(RiverSite(HexCoordinate(-1, 0), HexCoordinate(-2, 1), RiverType::stream));
    res.insert(RiverSite(HexCoordinate(-1, 0), HexCoordinate(-1, 1), RiverType::stream));
    res.insert(RiverSite(HexCoordinate(0, 0), HexCoordinate(-1, 1), RiverType::stream));
    res.insert(RiverSite(HexCoordinate(-1, 1), HexCoordinate(0, 1), RiverType::stream));
    return res;
}

TEST_CASE("hexagons") {
    SUBCASE("equality") {
        HexCoordinate coord1(1, 0, -1);
        HexCoordinate coord2(1, 0, -1);
        HexCoordinate coord3(2, -1, -1);

        CHECK_EQ(coord1, coord2);
        CHECK_NE(coord1, coord3);
    }

    SUBCASE("addition") {
        HexCoordinate coord1(3, 2);
        HexCoordinate coord2(5, 1);
        HexCoordinate coord3(8, 3);

        CHECK_EQ(coord1 + coord2, coord3);
    }

    SUBCASE("origin") {
        HexCoordinate coord1(0, 0);
        HexCoordinate coord2 = HexCoordinate::origin();

        CHECK_EQ(coord1, coord2);
    }

    SUBCASE("neighbors") {
        HexCoordinate coord(2, -1, -1);
        HexCoordinate coord_n = coord.neighbor(3);
        HexCoordinate coord_n_corect(1, 0, -1);

        CHECK_EQ(coord_n, coord_n_corect);
    }

    SUBCASE("distance") {
        HexCoordinate coord1(1, 0, -1);
        HexCoordinate coord2(3, -2, -1);

        CHECK_EQ(distance(coord1, coord2), 2);
    }
}

TEST_CASE("graph") {
    SUBCASE("BidirectionalGraph") {
        BidirectionalGraph<int> bg;
        bg.insert_node(0)
            .insert_node(1)
            .insert_node(2, {1})
            .insert_node(3, {1, 2})
            .insert_node(4, {0, 3})
            .insert_edge(3, 5);

        std::map<int, std::set<int>> expected = {
            {0, {4}}, {1, {2, 3}}, {2, {1, 3}}, {3, {1, 2, 4, 5}}, {4, {0, 3}}, {5, {3}}};

        CHECK_EQ(bg.adjacency_matrix(), expected);

        bg.remove_node(4);
        expected = {{0, {}}, {1, {2, 3}}, {2, {1, 3}}, {3, {1, 2, 5}}, {5, {3}}};

        CHECK_EQ(bg.adjacency_matrix(), expected);

        bg.remove_edge(1, 3);
        expected = {{0, {}}, {1, {2}}, {2, {1, 3}}, {3, {2, 5}}, {5, {3}}};

        CHECK_EQ(bg.adjacency_matrix(), expected);
    }

    SUBCASE("WeightedBidirectionalGraph") {
        BidirectionalGraph<int> bg;
        bg.insert_node(0)
            .insert_node(1)
            .insert_node(2, {1})
            .insert_node(3, {1, 2})
            .insert_node(4, {0, 3})
            .insert_edge(3, 5);

        WeightedBidirectionalGraph<int, int> wbg_expected(bg, 1);

        WeightedBidirectionalGraph<int, int> wbg;
        wbg.insert_edge(1, 2, 1, 1)
            .insert_edge(1, 3, 1, 1)
            .insert_edge(0, 4, 1, 1)
            .insert_edge(2, 3, 1, 1)
            .insert_edge(3, 4, 1, 1)
            .insert_edge(3, 5, 1, 1);
        CHECK_EQ(wbg.adjacency_matrix(), wbg_expected.adjacency_matrix());

        auto expected_adj = wbg.adjacency_matrix();
        wbg.insert_node(6);
        expected_adj[6] = {};
        CHECK_EQ(wbg.adjacency_matrix(), expected_adj);

        wbg = WeightedBidirectionalGraph(bg, 1);
        bg.remove_node(4);
        wbg_expected = WeightedBidirectionalGraph(bg, 1);
        wbg.remove_node(4);
        CHECK_EQ(wbg.adjacency_matrix(), wbg_expected.adjacency_matrix());

        wbg = WeightedBidirectionalGraph(bg, 1);
        bg.remove_edge(1, 2);
        wbg_expected = WeightedBidirectionalGraph(bg, 1);
        wbg.remove_edge(1, 2);
        CHECK_EQ(wbg.adjacency_matrix(), wbg_expected.adjacency_matrix());

        bg = BidirectionalGraph();
        bg.insert_node(0)
            .insert_node(1)
            .insert_node(2, {1})
            .insert_node(3, {1, 2})
            .insert_node(4, {0, 3});

        wbg_expected = WeightedBidirectionalGraph(bg, 0);

        wbg_expected.change_edge_weight(1, 2, 3)
            .change_edge_weight(1, 3, 0)
            .change_edge_weight(0, 4, 1)
            .change_edge_weight(2, 3, 0)
            .change_edge_weight(3, 4, 1)
            .change_edge_weight(2, 1, 4)
            .change_edge_weight(3, 1, 4)
            .change_edge_weight(4, 0, 3)
            .change_edge_weight(3, 2, 3)
            .change_edge_weight(4, 3, 0);

        wbg = WeightedBidirectionalGraph();
        wbg.insert_edge(1, 2, 3, 4)
            .insert_edge(1, 3, 0, 4)
            .insert_edge(0, 4, 1, 3)
            .insert_edge(2, 3, 0, 3)
            .insert_edge(3, 4, 1, 0);

        CHECK_EQ(wbg, wbg_expected);
    }

    SUBCASE("dijkstra") {
        WeightedBidirectionalGraph<int, int> wbg;
        wbg.insert_edge(1, 2, 3, 4)
            .insert_edge(1, 3, 0, 4)
            .insert_edge(0, 4, 1, 3)
            .insert_edge(2, 3, 0, 3)
            .insert_edge(3, 4, 1, 0);

        auto [dist, prev] = wbg.dijkstra(3);

        std::map<int, int> dist_expected = {{0, 4}, {1, 4}, {2, 3}, {3, 0}, {4, 1}};
        std::map<int, int> prev_expected = {{0, 4}, {1, 3}, {2, 3}, {4, 3}};

        CHECK_EQ(dist, dist_expected);
        CHECK_EQ(prev, prev_expected);
    }
}

TEST_CASE("site types") {
    SUBCASE("river") {
        CHECK_NOTHROW(
            RiverSite(HexCoordinate(1, -1), HexCoordinate(0, 0), RiverType::stream));
        CHECK_NOTHROW(
            RiverSite(HexCoordinate(1, 0), HexCoordinate(0, 1), RiverType::stream));
        CHECK_THROWS(
            RiverSite(HexCoordinate(1, -1), HexCoordinate(-1, 0), RiverType::stream));
        CHECK_THROWS(
            RiverSite(HexCoordinate(0, -1), HexCoordinate(0, 1), RiverType::stream));
    }
}

TEST_CASE("map") {
    SUBCASE("insertions") {
        Map map{};
        for (int r = -1; r <= 1; ++r) {
            for (int q = -1; q <= 1; ++q) {
                map.insert(HexSite(HexCoordinate(q, r), HexType::field));
            }
        }
        BidirectionalGraph<int> graph{};

        graph.insert_node(0)
            .insert_node(1, {0})
            .insert_node(2, {1})
            .insert_node(3, {0, 1})
            .insert_node(4, {3, 1, 2})
            .insert_node(5, {4, 2})
            .insert_node(6, {3, 4})
            .insert_node(7, {6, 4, 5})
            .insert_node(8, {5, 7});

        CHECK_EQ(map.graph(), graph);

        map.insert(
               RiverSite(HexCoordinate(0, -1), HexCoordinate(1, -1), RiverType::stream))
            .insert(
                RiverSite(HexCoordinate(0, 0), HexCoordinate(1, -1), RiverType::stream))
            .insert(
                RiverSite(HexCoordinate(0, 0), HexCoordinate(1, 0), RiverType::stream))
            .insert(
                RiverSite(HexCoordinate(1, 0), HexCoordinate(0, 1), RiverType::stream));

        CHECK_THROWS(map.insert(
            RiverSite(HexCoordinate(8, -1), HexCoordinate(9, -1), RiverType::stream)));

        graph.remove_edge(1, 2)
            .remove_edge(2, 4)
            .remove_edge(4, 5)
            .remove_edge(5, 7)
            .insert_node(9, {1, 2})
            .insert_node(10, {2, 4})
            .insert_node(11, {4, 5})
            .insert_node(12, {5, 7});

        CHECK_EQ(map.graph(), graph);
    }

    SUBCASE("controlable hexes") {
        const auto map               = simple_test_map();
        const auto neighbors         = map.get_controlable_hexes_from(5);
        const std::set<int> expected = {1, 2, 4, 6, 8, 9};

        CHECK_EQ(neighbors, expected);
    }

    SUBCASE("id and coords") {
        const auto map = simple_test_map();

        CHECK_EQ(map.get_hex_coord(7), HexCoordinate(-2, 1));

        CHECK_EQ(map.get_hex_id(HexCoordinate(1, -1)), 2);
    }
}
