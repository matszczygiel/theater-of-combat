#pragma once

#include <map>
#include <memory>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

#include "hex_site.h"
#include "map_site.h"
#include "passage_site.h"

#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

class Map {
   public:
    void draw(sf::RenderTarget& target) const;

    std::shared_ptr<Hex_site>& get_hex(const int& no);
    std::shared_ptr<Hex_site>& get_hex(const int& x, const int& y);
    std::shared_ptr<Passage_site>& get_pass(const int& no);

    void load(const std::string& path, const float& size);
    void save(const std::string& path) const;

    static Map create_test_map(const float& size);

    void set_numbers_drawing(const std::string& font_filename);
    void generate_plain_map(const float& xdim, const float& ydim, const float& site_size);

    template <class T>
    void connect_site(const int& hex1_no, const int& hex2_no);

   private:
    void recompute_geometry(const float& size);
    void resize(const int& x, const int& y);
    void compute_adjacency_of_hexes();

    constexpr int get_no(const int& x, const int& y);

    std::map<int, std::vector<int>> _adjacency_matrix;
    std::vector<std::shared_ptr<Hex_site>> _hexes;
    std::map<int, std::shared_ptr<Passage_site>> _passages;

    int _x_dim = 0;
    int _y_dim = 0;

    int _current_max_no = 0;

    sf::Font _numbers_font;
    bool _draw_numbers = false;

    friend class Mover;
};

template <class T>
void Map::connect_site(const int& hex1_no, const int& hex2_no) {
    static_assert(std::is_base_of<Passage_site, T>::value, "Trying to connect non passage site.");
    ++_current_max_no;
    _passages[_current_max_no]         = std::make_unique<T>(_current_max_no);
    _adjacency_matrix[_current_max_no] = {hex1_no, hex2_no};

    std::replace(_adjacency_matrix.at(hex1_no).begin(), _adjacency_matrix.at(hex1_no).end(),
                 hex2_no, _current_max_no);
    std::replace(_adjacency_matrix.at(hex2_no).begin(), _adjacency_matrix.at(hex2_no).end(),
                 hex1_no, _current_max_no);

    _passages[_current_max_no]->set_shape(get_hex(hex1_no)->get_position(),
                                          get_hex(hex2_no)->get_position(),
                                          get_hex(hex1_no)->get_radius());
}
