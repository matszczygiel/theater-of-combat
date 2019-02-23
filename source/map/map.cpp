#include "map.h"

#include <sstream>

#include <pugixml.hpp>

#include "concrete_hex.h"
#include "concrete_passage.h"
#include "log.h"
#include "site_factory.h"

void Map::draw(sf::RenderTarget& target) const {
    for (const auto& x : _map)
        for (const auto& y : x)
            y->draw(target);

    for (const auto& x : _passages)
        x->draw(target);
}

void Map::recompute_geometry(const float& size) {
    GAME_INFO("Recomputing map geometry, size: {0}", size);

    constexpr float map_offset = 100.f;

    const auto smr = Hex_shape::get_small_radius(size);

    for (int x = 0; x < x_dim; ++x)
        for (int y = 0; y < y_dim; ++y) {
            if (y % 2 == 0) {
                get_hex(x, y)->set_shape(map_offset + 2 * x * smr, map_offset + 1.5 * y * size, size);
            } else {
                get_hex(x, y)->set_shape(map_offset + (2 * x + 1) * smr, map_offset + 1.5 * y * size, size);
            }
        }
}

void Map::resize(const int& x, const int& y) {
    _map.resize(x);
    for (auto& vec : _map)
        vec.resize(y);

    x_dim = x;
    y_dim = y;
}

// do something with this function
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

std::unique_ptr<Hex_site>& Map::get_hex(const int& x, const int& y) {
    if (x >= x_dim || y >= y_dim) {
        GAME_ERROR("Invalid hex number requested! x: {0} y; {1}.", x, y);
        assert(true);
    }

    return _map.at(x).at(y);
}

std::unique_ptr<Hex_site>& Map::get_hex(const int& no) {
    const int y = no % y_dim;
    const int x = (no - y) / x_dim;
    if (x >= x_dim || y >= y_dim) {
        GAME_ERROR("Invalid hex number requested!: {0}", no);
        assert(true);
    }

    return _map.at(x).at(y);
}

void Map::load_map(const std::string& path, const float& size) {
    GAME_INFO("Loading map from a file: {0}", path);

    pugi::xml_document doc;
    auto parsing_result = doc.load_file(path.c_str());

    if (parsing_result) {
        GAME_TRACE("Map sucesfully loaded.");
    } else {
        GAME_CRITICAL("Invlaid map file. Error description: {0}. Error offset: {1}",
                      parsing_result.description(), parsing_result.offset);
    }

    _map.clear();
    _passages.clear();

    auto hexes    = doc.child("map").child("hexagons_set");
    auto passages = doc.child("map").child("passages_set");

    GAME_TRACE("Loading hexagons set.");

    resize(hexes.attribute("x_size").as_int(), hexes.attribute("y_size").as_int());

    for (auto hex_node : hexes.children("hex")) {
        const auto no   = hex_node.attribute("number").as_int();
        const auto type = string_to_hex_type(hex_node.attribute("hex_type").value());
        get_hex(no)     = Map_site_factory::create_hex(type, no);
    }
    GAME_TRACE("Loading passges set.");

    _passages.resize(passages.attribute("size").as_int());

    for (auto pass : passages.children("passage")) {
        const auto no    = pass.attribute("number").as_int();
        const auto type  = string_to_passage_type(pass.attribute("passage_type").value());
        _passages.at(no) = Map_site_factory::create_passage(type, no);
    }

    GAME_TRACE("Loading hexagons sides.");

    for (auto hex : hexes.children("hex")) {
        auto sides_node = hex.child("sides_array");
        const auto no   = hex.attribute("number").as_int();
        for (auto single_side_node : sides_node.children("side")) {
            const auto dire      = string_to_direction(single_side_node.attribute("direction").value());
            const auto site_node = single_side_node.child("site");
            const auto site_type = string_to_site_type(site_node.attribute("site_type").value());
            const auto no_side   = site_node.attribute("number").as_int();

            switch (site_type) {
                case Site_type::hexagon:
                    get_hex(no)->set_side(dire, get_hex(no_side).get());
                    break;
                case Site_type::passage:
                    get_hex(no)->set_side(dire, _passages.at(no_side).get());
                    break;
                default:
                    GAME_CRITICAL("Unknown site type.");
                    throw std::runtime_error("Unknown site type.");
                    break;
            }
        }
    }

    recompute_geometry(size);

    GAME_TRACE("Loading passages sides.");

    for (auto pass : passages.children("passage")) {
        const auto no  = pass.attribute("number").as_int();
        auto side_node = pass.child("sides_array").child("side");

        const auto dire1 = string_to_direction(side_node.attribute("direction").value());
        const auto no1   = side_node.child("site").attribute("number").as_int();

        side_node = side_node.next_sibling("side");

        const auto dire2 = string_to_direction(side_node.attribute("direction").value());
        const auto no2   = side_node.child("site").attribute("number").as_int();

        _passages.at(no)->set_sides(dire1, get_hex(no1).get(), dire2, get_hex(no2).get());
    }
}

void Map::save_map(const std::string& path) {
    GAME_INFO("Saving map to a file: {0}", path);

    pugi::xml_document doc;
    auto map_node = doc.append_child("map");

    auto hexes = map_node.append_child("hexagons_set");
    GAME_TRACE("Writing hexagons set.");

    hexes.append_attribute("x_size").set_value(x_dim);
    hexes.append_attribute("y_size").set_value(y_dim);

    for (int x = 0; x < x_dim; ++x)
        for (int y = 0; y < y_dim; ++y) {
            const auto& hex = get_hex(x, y);
            hex->write(hexes);
        }

    auto passes = map_node.append_child("passages_set");
    GAME_TRACE("Writing passages set.");

    passes.append_attribute("size").set_value(_passages.size());

    for (int p = 0; p < _passages.size(); ++p) {
        const auto& pass = _passages[p];
        pass->write(passes);
    }

    if (!doc.save_file(path.c_str())) {
        GAME_ERROR("Could not save map file.");
    }
}