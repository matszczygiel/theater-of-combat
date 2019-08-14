#include "map.h"

#include <algorithm>

const std::map<int, HexSite>& Map::hexes() const { return _hexes; }

const std::map<int, RiverSite>& Map::rivers() const { return _rivers; }

void Map::insert(HexSite site) {
    if (!std::any_of(_hexes.begin(), _hexes.end(), [&](const auto& x) {
            return x.second.coord() == site.coord();
        }))
        throw std::runtime_error("Map already contains such hex.");

    const auto neighors = site.coord().neighbors();

    std::map<int, HexSite> found_neighbors;

    std::copy_if(_hexes.begin(), _hexes.end(), std::inserter(found_neighbors),
                 [](cosnt auto& s) {
                     return std::any_of(
                         neighors.begin(), neighors.end(), [&](const auto& x) {
                             return x.second.coord() == s.second.coord();
                         })
                 });

    _hexes.insert({_current_free_id++, site});

    102   │ let neighbours = hex.coord.neighbors();
    103   │ 104   │ let found_neighbors
        : HashMap<_, _> =
              self 105   │ .hexes 106   │ .clone() 107   │ .into_iter() 108   │
                  .filter(| (id, h) | {
                      109   │ if neighbours.contains(&h.coord) {
                          110   │ return true;
                          111   │
                      }
                      else {
                          112   │ return false;
                          113   │
                      }
                      114   │
                  }) 115   │ .collect();
    116   │ 117   │ self.hexes.insert(self.current_free_id, hex);
    118   │ 119   │ self.graph.insert_node(
        120   │ self.current_free_id,
        121   │ found_neighbors.keys().cloned().collect(), 122   │ )
        ? ;
    123   │ self.current_free_id += 1;
    124   │ Ok(self)
}

void Map::insert(RiverSite site) {}

/*
Passage_site& Map::get_pass(const int& no) {
    if (no < static_cast<int>(_hexes.size()) || no > _current_max_no) {
        ENGINE_ERROR("Invalid pass number requested!: {0}, must by in domain
[{1}, {2}].", no, _hexes.size() + 1, _current_max_no); assert(false);
    }
    return *_passages.at(no);
}

constexpr int Map::get_no(const int& x, const int& y) {
    return _x_dim * x + y;
}

Hex_site& Map::get_hex(const int& x, const int& y) {
    if (x >= _x_dim || y >= _y_dim) {
        ENGINE_ERROR("Invalid hex number requested! x: {0} y; {1}. Curent
values: {2}, {3}.", x, y, _x_dim, _y_dim); assert(false);
    }
    const int no = get_no(x, y);
    return get_hex(no);
}

Hex_site& Map::get_hex(const int& no) {
    if (no >= static_cast<int>(_hexes.size())) {
        ENGINE_ERROR("Invalid hex number requested!: {0}, current number of
hexes: {1}.", no, _hexes.size()); assert(false); return _hexes.back();
    }
    return _hexes.at(no);
}

void Map::draw(sf::RenderTarget& target) const {
    for (const auto& x : _hexes) {
        x.draw(target);
    }

    for (const auto& x : _passages) {
        x.second->draw(target);
    }

    if (_draw_numbers) {
        for (const auto& x : _hexes) {
            x.draw_number(target, _numbers_font);
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
                get_hex(x, y).set_shape({map_offset + 2 * x * smr, map_offset
+ 1.5f * y * size}, size); } else { get_hex(x, y).set_shape({map_offset + (2 * x
+ 1) * smr, map_offset + 1.5f * y * size}, size);
            }
        }

    for (auto& x : _passages) {
        x.second->set_shape(get_hex(_adjacency_matrix.at(x.first).at(0)).get_position(),
                            get_hex(_adjacency_matrix.at(x.first).at(1)).get_position(),
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

void Map::generate_plain_map(const float& xdim, const float& ydim, const float&
site_size) { ENGINE_INFO("Creating plain map of size: {0} x {1}", xdim, ydim);

    resize(xdim, ydim);

    for (int x = 0; x < xdim; ++x)
        for (int y = 0; y < ydim; ++y) {
            const auto no = get_no(x, y);
            get_hex(no)   = Hex_site(no);
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
                        _adjacency_matrix[get_no(x, y)].push_back(get_no(x - 1,
y - 1)); _adjacency_matrix[get_no(x - 1, y - 1)].push_back(get_no(x, y));
                    }
                }
            } else {
                _adjacency_matrix[get_no(x, y)].push_back(get_no(x, y - 1));
                _adjacency_matrix[get_no(x, y - 1)].push_back(get_no(x, y));

                if (x != _x_dim - 1) {
                    _adjacency_matrix[get_no(x, y)].push_back(get_no(x + 1, y -
1)); _adjacency_matrix[get_no(x + 1, y - 1)].push_back(get_no(x, y));
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
            res.get_hex(x, y) = Hex_site(res.get_no(x, y),
Hex_site::Type::Forest);
        }
    res.recompute_geometry(size);

    for (int i = 0; i < 10; ++i) {
        res.connect_site<River>(res.get_no(5, i), res.get_no(6, i));
    }

    for (int i = 0; i < 5; ++i) {
        res.connect_site<River>(res.get_no(5, 2 * i + 1), res.get_no(6, 2 * i));
    }

    for (int i = 0; i < 4; ++i) {
        res.connect_site<River>(res.get_no(5, 1 + 2 * i), res.get_no(6, 2 + 2 *
i));
    }

    return res;
}

void Map::set_numbers_drawing(const std::string& font_filename) {
    _numbers_font.loadFromFile(font_filename);
    _draw_numbers = true;
}

std::set<int> Map::get_controlable_hexes_from(const int& src) const {
    ENGINE_ASSERT(src < static_cast<int>(_hexes.size()), "Invalid source hex no
given!"); auto vec           = _adjacency_matrix.at(src); const int hex_size =
_hexes.size();

    std::set<int> res;
    for (auto& v : vec) {
        if (v < hex_size)
            res.insert(v);
    }
    return res;
}*/