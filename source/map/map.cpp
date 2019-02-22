#include "map.h"

#include <sstream>

#include <pugixml.hpp>

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

void Map::load_map(const std::string& path) {
}

void Map::save_map(const std::string& path) {
    GAME_INFO("Saving map to a file: {0}", path);

    pugi::xml_document doc;

    auto pass  = doc.append_child("passages");
    auto hexes = doc.append_child("hexagons");

    pass.append_attribute("passages size").set_value(_passages.size());
    hexes.append_attribute("x size").set_value(_map.size());
    hexes.append_attribute("y size").set_value(_map.at(0).size());

    for (int x = 0; x < _map.size(); ++x)
        for (int y = 0; y < _map.at(0).size(); ++y) {
            const auto& hex = _map[x][y];
            auto hex_node   = hexes.append_child("hex");

            hex_node.append_attribute("x").set_value(x);
            hex_node.append_attribute("y").set_value(y);
            hex_node.append_attribute("number").set_value(hex->get_number());
            hex_node.append_attribute("type").set_value(static_cast<int>(hex->get_hex_type()));

            auto sides_node = hex_node.append_child("sides array");

            for (int s = 0; s < 6; ++s) {
                const auto& side = hex->get_side(static_cast<Directions>(s));

                if (side != nullptr) {
                    auto single_side_node = sides_node.append_child("side");
                    single_side_node.append_attribute("direction").set_value(s);
                    single_side_node.append_attribute("type").set_value(static_cast<int>(side->get_site_type()));
                    single_side_node.append_attribute("number").set_value(side->get_number());
                }
            }
        }

    for (int p = 0; p < _passages.size(); ++p) {
        const auto& pass = _passages[p];
        auto pass_node   = hexes.append_child("passage");

        pass_node.append_attribute("number").set_value(pass->get_number());
        pass_node.append_attribute("type").set_value(static_cast<int>(pass->get_passage_type()));

        const auto& sides     = pass->get_sides();
        const auto& sides_dir = pass->get_sides_directions();
        const auto& side0     = std::get<0>(sides);
        const auto& side1     = std::get<1>(sides);
        const auto& side0_dir = std::get<0>(sides_dir);
        const auto& side1_dir = std::get<1>(sides_dir);
        auto sides_node       = pass_node.append_child("sides array");

        if (side0 != nullptr) {
            auto single_side_node0 = sides_node.append_child("side");
            single_side_node0.append_attribute("direction").set_value(static_cast<int>(side0_dir));
            single_side_node0.append_attribute("type").set_value(static_cast<int>(Site_type::hexagon));
            single_side_node0.append_attribute("number").set_value(side0->get_number());
        }

        if (side1 != nullptr) {
            auto single_side_node1 = sides_node.append_child("side");
            single_side_node1.append_attribute("direction").set_value(static_cast<int>(side1_dir));
            single_side_node1.append_attribute("type").set_value(static_cast<int>(Site_type::hexagon));
            single_side_node1.append_attribute("number").set_value(side1->get_number());
        }
    }
    doc.save_file(path.c_str());
}