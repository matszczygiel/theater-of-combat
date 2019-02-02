#include "map.h"

#include "concrete_hex.h"
#include "log.h"

void Map::draw(sf::RenderTarget& target) const {
    for (const auto& x : _map)
        for (const auto& y : x)
            y->draw(target);
}

Map Map::create_test_map(const float& size) {
    GAME_INFO("Creating test map of size: {0}", size);

    constexpr int dim = 10;
    Map res;
    res._map.resize(dim);

    const auto smr = Hex_shape::get_small_radius(size);

    for (int x = 0; x < dim; ++x)
        for (int y = 0; y < dim; ++y) {
            const int no = 10 * x + y;
            if (x < 4 && y < 6) {
                res._map[x].emplace_back(new Forest(no));
            } else {
                res._map[x].emplace_back(new Field(no));
            }
            if (y % 2 == 0) {
                res._map[x][y]->set_shape(100 + 2 * x * smr, 100 + 1.5 * y * size, size);
            } else {
                res._map[x][y]->set_shape(100 + (2 * x + 1) * smr, 100 + 1.5 * y * size, size);
            }
        }

    for (int x = 0; x < dim; ++x)
        for (int y = 0; y < dim; ++y) {
            if (y % 2 == 0) {
                if (y != 0) {
                    res._map[x][y]->set_side(Directions::northeast, res._map[x][y - 1].get());
                    if (x != 0)
                        res._map[x][y]->set_side(Directions::northwest, res._map[x - 1][y - 1].get());
                }
            } else {
                res._map[x][y]->set_side(Directions::northwest, res._map[x][y - 1].get());
                if (x != dim - 1)
                    res._map[x][y]->set_side(Directions::northeast, res._map[x + 1][y - 1].get());
            }
            if (x != 0)
                res._map[x][y]->set_side(Directions::west, res._map[x - 1][y].get());
        }

    return res;
}

Hex_site* Map::get_hex(const int& no) {
    const int x = no / _map.size();
    if (x >= _map.size()) {
        GAME_ERROR("Invalid hex number requested!: {0}", no);
        assert(true);
    }
    const int y = no % _map[x].size();
    if (y >= _map[x].size()) {
        GAME_ERROR("Invalid hex number requested!: {0}", no);
        assert(true);
    }

    return _map[x][y].get();
}
