#include "map.h"

#include "concrete_hex.h"
#include "concrete_passage.h"
#include "log.h"

void Map::draw(sf::RenderTarget& target) const {
    for (const auto& x : _map)
        for (const auto& y : x)
            y->draw(target);

    for (const auto& x : _passages)
        x->draw(target);
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
                res._map[x].emplace_back(std::make_unique<Forest>(no));
            } else {
                res._map[x].emplace_back(std::make_unique<Field>(no));
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
                    res._map[x][y]
                        ->set_side(Directions::northeast, res._map[x][y - 1].get());
                    res._map[x][y - 1]
                        ->set_side(Map_site::opposite_direction(Directions::northeast), res._map[x][y].get());
                    if (x != 0) {
                        res._map[x][y]
                            ->set_side(Directions::northwest, res._map[x - 1][y - 1].get());
                        res._map[x - 1][y - 1]
                            ->set_side(Map_site::opposite_direction(Directions::northwest), res._map[x][y].get());
                    }
                }
            } else {
                res._map[x][y]
                    ->set_side(Directions::northwest, res._map[x][y - 1].get());
                res._map[x][y - 1]
                    ->set_side(Map_site::opposite_direction(Directions::northwest), res._map[x][y].get());
                if (x != dim - 1) {
                    res._map[x][y]
                        ->set_side(Directions::northeast, res._map[x + 1][y - 1].get());
                    res._map[x + 1][y - 1]
                        ->set_side(Map_site::opposite_direction(Directions::northeast), res._map[x][y].get());
                }
            }
            if (x != 0) {
                res._map[x][y]
                    ->set_side(Directions::west, res._map[x - 1][y].get());
                res._map[x - 1][y]
                    ->set_side(Map_site::opposite_direction(Directions::west), res._map[x][y].get());
            }
        }

    for (int i = 0; i < 10; ++i) {
        const Directions dir = Directions::east;
        res._passages.push_back(std::make_unique<River>(i));
        res._passages[i]->set_sides(dir,
                                    res._map[6][i].get(),
                                    Map_site::opposite_direction(dir),
                                    static_cast<Hex_site*>(res._map[6][i]->get_side(Map_site::opposite_direction(dir))));
    }
    for (int i = 0; i < 5; ++i) {
        const Directions dir = Directions::northeast;
        res._passages.push_back(std::make_unique<River>(10 + i));
        res._passages[10 + i]->set_sides(dir,
                                         res._map[6][2 * i].get(),
                                         Map_site::opposite_direction(dir),
                                         static_cast<Hex_site*>(res._map[6][2 * i]->get_side(Map_site::opposite_direction(dir))));
    }

    for (int i = 0; i < 4; ++i) {
        const Directions dir = Directions::southeast;
        res._passages.push_back(std::make_unique<River>(15 + i));
        res._passages[15 + i]->set_sides(dir,
                                         res._map[6][2 + 2 * i].get(),
                                         Map_site::opposite_direction(dir),
                                         static_cast<Hex_site*>(res._map[6][2 + 2 * i]->get_side(Map_site::opposite_direction(dir))));
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
