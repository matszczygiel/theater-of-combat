#include "map.h"

#include <algorithm>

#include <pugixml.hpp>

#include "concrete_hex.h"
#include "concrete_passage.h"
#include "log.h"

std::shared_ptr<Passage_site>& Map::get_pass(const int& no) {
    if (no <= static_cast<int>(_hexes.size()) || no > _current_max_no) {
        ENGINE_ERROR("Invalid pass number requested!: {0}, must by in domain [{1}, {2}].",
                   no, _hexes.size(), _passages.size());
        assert(true);
    }
    return _passages.at(no);
}

constexpr int Map::get_no(const int& x, const int& y) {
    return _x_dim * x + y;
}

std::shared_ptr<Hex_site>& Map::get_hex(const int& x, const int& y) {
    if (x >= _x_dim || y >= _y_dim) {
        ENGINE_ERROR("Invalid hex number requested! x: {0} y; {1}. Curent values: {2}, {3}.",
                   x, y, _x_dim, _y_dim);
        assert(true);
    }
    const int no = get_no(x, y);
    return get_hex(no);
}

std::shared_ptr<Hex_site>& Map::get_hex(const int& no) {
    if (no >= static_cast<int>(_hexes.size())) {
        ENGINE_ERROR("Invalid hex number requested!: {0}, current number of hexes: {1}.",
                   no, _hexes.size());
        assert(true);
        return _hexes.back();
    }
    return _hexes.at(no);
}

void Map::draw(sf::RenderTarget& target) const {
    for (const auto& x : _hexes) {
        x->draw(target);
    }

    for (const auto& x : _passages) {
        x.second->draw(target);
    }

    if (_draw_numbers) {
        for (const auto& x : _hexes) {
            x->draw_number(target, _numbers_font);
        }
    }
}

void Map::recompute_geometry(const float& size) {
    ENGINE_INFO("Recomputing map geometry, size: {0}", size);

    constexpr float map_offset = 100.f;

    const auto smr = Hex_shape::get_small_radius(size);

    for (int x = 0; x < _x_dim; ++x)
        for (int y = 0; y < _y_dim; ++y) {
            if (y % 2 == 0) {
                get_hex(x, y)->set_shape(map_offset + 2 * x * smr, map_offset + 1.5 * y * size, size);
            } else {
                get_hex(x, y)->set_shape(map_offset + (2 * x + 1) * smr, map_offset + 1.5 * y * size, size);
            }
        }

    for (auto& x : _passages) {
        x.second->set_shape(get_hex(_adjacency_matrix.at(x.first).at(0))->get_position(),
                            get_hex(_adjacency_matrix.at(x.first).at(1))->get_position(),
                            size);
    }
}

void Map::resize(const int& x, const int& y) {
    _x_dim          = x;
    _y_dim          = y;
    _current_max_no = x * y - 1;

    _hexes.clear();
    _passages.clear();
    _adjacency_matrix.clear();

    _hexes.resize(x * y);
}

void Map::generate_plain_map(const float& xdim, const float& ydim, const float& site_size) {
    ENGINE_INFO("Creating plain map of size: {0} x {1}", xdim, ydim);

    resize(xdim, ydim);

    for (int x = 0; x < xdim; ++x)
        for (int y = 0; y < ydim; ++y) {
            const auto no = get_no(x, y);
            get_hex(no)   = std::make_unique<Field>(no);
        }

    recompute_geometry(site_size);
    compute_adjacency_of_hexes();
}

void Map::compute_adjacency_of_hexes() {
    for (int x = 0; x < _x_dim; ++x)
        for (int y = 0; y < _y_dim; ++y) {
            if (y % 2 == 0) {
                if (y != 0) {
                    _adjacency_matrix[get_no(x, y)].push_back(get_no(x, y - 1));
                    _adjacency_matrix[get_no(x, y - 1)].push_back(get_no(x, y));

                    if (x != 0) {
                        _adjacency_matrix[get_no(x, y)].push_back(get_no(x - 1, y - 1));
                        _adjacency_matrix[get_no(x - 1, y - 1)].push_back(get_no(x, y));
                    }
                }
            } else {
                _adjacency_matrix[get_no(x, y)].push_back(get_no(x, y - 1));
                _adjacency_matrix[get_no(x, y - 1)].push_back(get_no(x, y));

                if (x != _x_dim - 1) {
                    _adjacency_matrix[get_no(x, y)].push_back(get_no(x + 1, y - 1));
                    _adjacency_matrix[get_no(x + 1, y - 1)].push_back(get_no(x, y));
                }
            }
            if (x != 0) {
                _adjacency_matrix[get_no(x, y)].push_back(get_no(x - 1, y));
                _adjacency_matrix[get_no(x - 1, y)].push_back(get_no(x, y));
            }
        }
}

Map Map::create_test_map(const float& size) {
    ENGINE_INFO("Creating test map of size: {0}", size);

    constexpr int dim = 10;
    Map res;
    res.generate_plain_map(dim, dim, size);

    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 7; ++y) {
            res.get_hex(x, y) = std::make_unique<Forest>(res.get_no(x, y));
        }
    res.recompute_geometry(size);

    for (int i = 0; i < 10; ++i) {
        res.connect_site<River>(res.get_no(5, i), res.get_no(6, i));
    }

    for (int i = 0; i < 5; ++i) {
        res.connect_site<River>(res.get_no(5, 2 * i + 1), res.get_no(6, 2 * i));
    }

    for (int i = 0; i < 4; ++i) {
        res.connect_site<River>(res.get_no(5, 1 + 2 * i), res.get_no(6, 2 + 2 * i));
    }

    return res;
}

void Map::load(const std::string& path, const float& size) {
    ENGINE_INFO("Loading map from a file: {0}", path);

    pugi::xml_document doc;
    auto parsing_result = doc.load_file(path.c_str());

    if (parsing_result) {
        ENGINE_TRACE("Map file sucesfully loaded.");
    } else {
        ENGINE_CRITICAL("Invlaid map file. Error description: {0}. Error offset: {1}",
                        parsing_result.description(), parsing_result.offset);
    }

    const auto map_node = doc.child("map");

    const auto x_size = map_node.attribute("x-size").as_int();
    const auto y_size = map_node.attribute("y-size").as_int();

    resize(x_size, y_size);

    auto sites_node = map_node.child("sites");

    for (auto node : sites_node.children("map-site")) {
        std::shared_ptr<Map_site> site = Map_site::unserialize(node);
        const auto& no                 = site->get_number();
        if (no < static_cast<int>(_hexes.size())) {
            _hexes[no] = std::dynamic_pointer_cast<Hex_site>(site);
        } else {
            _passages[no] = std::dynamic_pointer_cast<Passage_site>(site);
        }
    }

    auto adj_node = map_node.child("adjacency");

    for (auto x : adj_node.children("site")) {
        const auto id         = x.attribute("id").as_int();
        _adjacency_matrix[id] = {};
        for (auto y : x.child("vec").attributes())
            _adjacency_matrix[id].push_back(y.as_int());
    }

    recompute_geometry(size);
    ENGINE_INFO("Map successfully loaded.");
}

void Map::save(const std::string& path) const {
    ENGINE_INFO("Saving map to a file: {0}", path);

    pugi::xml_document doc;
    auto map_node = doc.append_child("map");
    map_node.append_attribute("x-size").set_value(_x_dim);
    map_node.append_attribute("y-size").set_value(_y_dim);

    auto sites = map_node.append_child("sites");
    ENGINE_TRACE("Writing sites set.");

    for (const auto& x : _hexes)
        x->serialize(sites);

    for (const auto& x : _passages)
        x.second->serialize(sites);

    auto adj = map_node.append_child("adjacency");

    for (const auto& x : _adjacency_matrix) {
        auto node = adj.append_child("site");
        node.append_attribute("id").set_value(x.first);
        auto vec_node = node.append_child("vec");
        for (const auto& y : x.second) {
            vec_node.append_attribute("entry").set_value(y);
        }
    }

    if (!doc.save_file(path.c_str())) {
        ENGINE_ERROR("Could not save map file.");
    }
}

void Map::set_numbers_drawing(const std::string& font_filename) {
    _numbers_font.loadFromFile(font_filename);
    _draw_numbers = true;
}