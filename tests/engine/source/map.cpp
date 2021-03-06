#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "toc/map/graph.h"
#include "toc/map/hexagons.h"
#include "toc/map/map.h"
#include "toc/map/types.h"

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& value) {
    os << "( " << value.first << ", " << value.second << ')';
    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const BidirectionalGraph<std::pair<Map::SiteId, int>>& value) {
    os << '{';
    for (const auto& [node, neighbors] : value.adjacency_matrix()) {
        os << "{ " << node << ", {";
        for (const auto& n : neighbors)
            os << ' ' << n;
        os << " }}\n";
    }
    os << '}';
    return os;
}

static Map simple_test_map() {
    Map res;
    // hexes
    res.insert(HexSite(HexCoordinate(-1, -1), 0));
    res.insert(HexSite(HexCoordinate(0, -1), 1));
    res.insert(HexSite(HexCoordinate(1, -1), 1));
    res.insert(HexSite(HexCoordinate(-2, 0), 0));
    res.insert(HexSite(HexCoordinate(-1, 0), 0));
    res.insert(HexSite(HexCoordinate(0, 0), 1));
    res.insert(HexSite(HexCoordinate(1, 0), 1));
    res.insert(HexSite(HexCoordinate(-2, 1), 0));
    res.insert(HexSite(HexCoordinate(-1, 1), 0));
    res.insert(HexSite(HexCoordinate(0, 1), 0));

    // borders
    res.insert(BorderSite(HexCoordinate(-1, -1), HexCoordinate(-2, 0), 0));
    res.insert(BorderSite(HexCoordinate(-1, 0), HexCoordinate(-2, 0), 0));
    res.insert(BorderSite(HexCoordinate(-1, 0), HexCoordinate(-2, 1), 1));
    res.insert(BorderSite(HexCoordinate(-1, 0), HexCoordinate(-1, 1), 1));
    res.insert(BorderSite(HexCoordinate(0, 0), HexCoordinate(-1, 1), 1));
    res.insert(BorderSite(HexCoordinate(-1, 1), HexCoordinate(0, 1), 1));
    return res;
}

TEST_CASE("hexagons") {
    SUBCASE("equality") {
        HexCoordinate coord1(1, -1);
        HexCoordinate coord3(2, -1);

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
        HexCoordinate coord(2, -1);
        HexCoordinate coord_n = coord.neighbor(3);
        HexCoordinate coord_n_corect(1, -1);

        CHECK_EQ(coord_n, coord_n_corect);
    }

    SUBCASE("distance") {
        HexCoordinate coord1(1, -1);
        HexCoordinate coord2(3, -1);

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

    SUBCASE("UnidirectionalGraph") {
        UnidirectionalGraph<int> ug;
        ug.insert_node(0)
            .insert_node(1)
            .insert_node(2, {1})
            .insert_node(3, {1, 2})
            .insert_node(4, {0, 3})
            .insert_edge(3, 5);

        std::map<int, std::set<int>> expected = {{0, {}},        {1, {}},     {2, {1}},
                                                 {3, {1, 2, 5}}, {4, {0, 3}}, {5, {}}};

        CHECK_EQ(ug.adjacency_matrix(), expected);

        ug.remove_node(2);
        expected = {{0, {}}, {1, {}}, {3, {1, 5}}, {4, {0, 3}}, {5, {}}};

        CHECK_EQ(ug.adjacency_matrix(), expected);

        ug.remove_edge(3, 1);
        expected = {{0, {}}, {1, {}}, {3, {5}}, {4, {0, 3}}, {5, {}}};

        CHECK_EQ(ug.adjacency_matrix(), expected);
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

    SUBCASE("WeightedUnidirectionalGraph") {
        UnidirectionalGraph<int> bg;
        bg.insert_node(0)
            .insert_node(1)
            .insert_node(2, {1})
            .insert_node(3, {1, 2})
            .insert_node(4, {0, 3})
            .insert_edge(3, 5);

        WeightedUnidirectionalGraph<int, int> wbg_expected(bg, 1);

        WeightedUnidirectionalGraph<int, int> wbg;
        wbg.insert_edge(2, 1, 1)
            .insert_edge(3, 1, 1)
            .insert_edge(4, 0, 1)
            .insert_edge(3, 2, 1)
            .insert_edge(4, 3, 1)
            .insert_edge(3, 5, 1);
        CHECK_EQ(wbg.adjacency_matrix(), wbg_expected.adjacency_matrix());

        auto expected_adj = wbg.adjacency_matrix();
        wbg.insert_node(6);
        expected_adj[6] = {};
        CHECK_EQ(wbg.adjacency_matrix(), expected_adj);

        wbg = WeightedUnidirectionalGraph(bg, 1);
        bg.remove_node(4);
        wbg_expected = WeightedUnidirectionalGraph(bg, 1);
        wbg.remove_node(4);
        CHECK_EQ(wbg.adjacency_matrix(), wbg_expected.adjacency_matrix());

        wbg = WeightedUnidirectionalGraph(bg, 1);
        bg.remove_edge(1, 2);
        wbg_expected = WeightedUnidirectionalGraph(bg, 1);
        wbg.remove_edge(1, 2);
        CHECK_EQ(wbg.adjacency_matrix(), wbg_expected.adjacency_matrix());

        bg = BidirectionalGraph();
        bg.insert_node(0)
            .insert_node(1)
            .insert_node(2, {1})
            .insert_node(3, {1, 2})
            .insert_node(4, {0, 3});

        wbg_expected = WeightedUnidirectionalGraph(bg, 0);

        wbg_expected.change_edge_weight(2, 1, 3)
            .change_edge_weight(3, 1, 0)
            .change_edge_weight(4, 0, 1)
            .change_edge_weight(3, 2, 0)
            .change_edge_weight(4, 3, 1);

        wbg = WeightedUnidirectionalGraph();
        wbg.insert_edge(2, 1, 3)
            .insert_edge(3, 1, 0)
            .insert_edge(4, 0, 1)
            .insert_edge(3, 2, 0)
            .insert_edge(4, 3, 1);

        CHECK_EQ(wbg, wbg_expected);
    }

    SUBCASE("dijkstra") {
        WeightedBidirectionalGraph<int, int> wbg;
        wbg.insert_edge(1, 2, 3, 4)
            .insert_edge(1, 3, 0, 4)
            .insert_edge(0, 4, 1, 3)
            .insert_edge(2, 3, 0, 3)
            .insert_edge(3, 4, 1, 0);

        const auto [dist, prev] = wbg.dijkstra(3);

        std::map<int, int> dist_expected = {{0, 4}, {1, 4}, {2, 3}, {3, 0}, {4, 1}};
        std::map<int, int> prev_expected = {{0, 4}, {1, 3}, {2, 3}, {4, 3}};

        CHECK_EQ(dist, dist_expected);
        CHECK_EQ(prev, prev_expected);
    }
}

TEST_CASE("site types") {
    SUBCASE("border") {
        CHECK_NOTHROW(BorderSite(HexCoordinate(1, -1), HexCoordinate(0, 0), 1));
        CHECK_NOTHROW(BorderSite(HexCoordinate(1, 0), HexCoordinate(0, 1), 1));
        CHECK_THROWS(BorderSite(HexCoordinate(1, -1), HexCoordinate(-1, 0), 1));
        CHECK_THROWS(BorderSite(HexCoordinate(0, -1), HexCoordinate(0, 1), 1));
    }
}

TEST_CASE("map") {
    Map map{};
    UnidirectionalGraph<std::pair<Map::SiteId, int>> graph{};

    map.insert(HexSite(HexCoordinate(0, -1), 0));
    map.insert(HexSite(HexCoordinate(1, -1), 0));
    map.insert(HexSite(HexCoordinate(-1, 0), 0));
    map.insert(HexSite(HexCoordinate(0, 0), 0));
    map.insert(HexSite(HexCoordinate(1, 0), 0));
    map.insert(HexSite(HexCoordinate(-1, 1), 0));
    map.insert(HexSite(HexCoordinate(0, 1), 0));

    for (int i = 0; i <= 6; ++i) {
        graph.insert_edge({i, 0}, {i, 1})
            .insert_edge({i, 1}, {i, 0})
            .insert_edge({i, 1}, {i, 2})
            .insert_edge({i, 2}, {i, 1})
            .insert_edge({i, 2}, {i, 3})
            .insert_edge({i, 3}, {i, 2})
            .insert_edge({i, 3}, {i, 4})
            .insert_edge({i, 4}, {i, 3})
            .insert_edge({i, 4}, {i, 5})
            .insert_edge({i, 5}, {i, 4})
            .insert_edge({i, 5}, {i, 0})
            .insert_edge({i, 0}, {i, 5});
    }

    graph.insert_edge({0, 0}, {1, 0})
        .insert_edge({0, 5}, {3, 5})
        .insert_edge({0, 4}, {2, 4})
        .insert_edge({1, 3}, {0, 3})
        .insert_edge({1, 4}, {3, 4})
        .insert_edge({1, 5}, {4, 5})
        .insert_edge({2, 0}, {3, 0})
        .insert_edge({2, 1}, {0, 1})
        .insert_edge({2, 5}, {5, 5})
        .insert_edge({3, 0}, {4, 0})
        .insert_edge({3, 1}, {1, 1})
        .insert_edge({3, 2}, {0, 2})
        .insert_edge({3, 3}, {2, 3})
        .insert_edge({3, 4}, {5, 4})
        .insert_edge({3, 5}, {6, 5})
        .insert_edge({4, 2}, {1, 2})
        .insert_edge({4, 3}, {3, 3})
        .insert_edge({4, 4}, {6, 4})
        .insert_edge({5, 0}, {6, 0})
        .insert_edge({5, 1}, {3, 1})
        .insert_edge({5, 2}, {2, 2})
        .insert_edge({6, 1}, {4, 1})
        .insert_edge({6, 2}, {3, 2})
        .insert_edge({6, 3}, {5, 3});

    SUBCASE("hex insertions") { CHECK_EQ(map.graph(), graph); }

    SUBCASE("borders insertions") {
        map.insert(BorderSite(HexCoordinate(0, -1), HexCoordinate(1, -1), 1))
            .insert(BorderSite(HexCoordinate(0, 0), HexCoordinate(1, -1), 1))
            .insert(BorderSite(HexCoordinate(0, 0), HexCoordinate(1, 0), 1))
            .insert(BorderSite(HexCoordinate(1, 0), HexCoordinate(0, 1), 1));

        CHECK_THROWS(
            map.insert(BorderSite(HexCoordinate(8, -1), HexCoordinate(9, -1), 1)));

        graph.remove_edge({0, 0}, {1, 0})
            .remove_edge({1, 3}, {0, 3})
            .insert_edge({0, 0}, {7, 0})
            .insert_edge({7, 0}, {1, 0})
            .insert_edge({1, 3}, {7, 3})
            .insert_edge({7, 3}, {0, 3})
            .remove_edge({3, 1}, {1, 1})
            .remove_edge({1, 4}, {3, 4})
            .insert_edge({3, 1}, {8, 1})
            .insert_edge({8, 1}, {1, 1})
            .insert_edge({1, 4}, {8, 4})
            .insert_edge({8, 4}, {3, 4})
            .remove_edge({3, 0}, {4, 0})
            .remove_edge({4, 3}, {3, 3})
            .insert_edge({3, 0}, {9, 0})
            .insert_edge({9, 0}, {4, 0})
            .insert_edge({4, 3}, {9, 3})
            .insert_edge({9, 3}, {3, 3})
            .remove_edge({6, 1}, {4, 1})
            .remove_edge({4, 4}, {6, 4})
            .insert_edge({6, 1}, {10, 1})
            .insert_edge({10, 1}, {4, 1})
            .insert_edge({4, 4}, {10, 4})
            .insert_edge({10, 4}, {6, 4});

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
