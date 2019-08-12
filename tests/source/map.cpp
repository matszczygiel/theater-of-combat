#include <gtest/gtest.h>

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