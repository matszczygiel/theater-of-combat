#include <gtest/gtest.h>

#include "../../source/map/graph.cpp"
#include "../../source/map/hexagons.cpp"

TEST(Hexagons, equality) {
    HexCoordinate coord1(3, -5, 2);
    HexCoordinate coord2(3, -5, 2);

    ASSERT_EQ(coord1, coord2);
}

TEST(Hexagons, addition) {
    HexCoordinate coord1(3, 2);
    HexCoordinate coord2(5, 1);
    HexCoordinate coord3(8, 3);

    ASSERT_EQ(coord1 + coord2, coord3);
}

TEST(Hexagons, origin) {
    HexCoordinate coord1(0, 0);
    HexCoordinate coord2 = HexCoordinate::origin();

    ASSERT_EQ(coord1, coord2);
}

TEST(Hexagons, neighbors) {
    HexCoordinate coord(2, -1, -1);
    HexCoordinate coord_n = coord.neighbor(3);
    HexCoordinate coord_n_corect(1, 0, -1);
    ASSERT_EQ(coord_n, coord_n_corect);
}

TEST(Hexagons, distance) {
    HexCoordinate coord1(1, 0, -1);
    HexCoordinate coord2(3, -2, -1);
    ASSERT_EQ(distance(coord1, coord2), 2);
}

TEST(Graph, inserting) {
    BidirectionalGraph bg;
    bg.insert_node(0, {})
        .insert_node(1, {})
        .insert_node(2, {1})
        .insert_node(3, {1, 2})
        .insert_node(4, {0, 3});

    std::map<int, std::set<int>> expected = {
        {0, {4}}, {1, {2, 3}}, {2, {1, 3}}, {3, {1, 2, 4}}, {4, {0, 3}}};

    ASSERT_EQ(bg.adjacency_matrix(), expected);
}

TEST(Graph, removing) {
    BidirectionalGraph bg;
    bg.insert_node(0, {})
        .insert_node(1, {})
        .insert_node(2, {1})
        .insert_node(3, {1, 2})
        .insert_node(4, {0, 3});

    bg.remove_node(4);

    std::map<int, std::set<int>> expected = {
        {0, {}}, {1, {2, 3}}, {2, {1, 3}}, {3, {1, 2}}};

    ASSERT_EQ(bg.adjacency_matrix(), expected);
}
