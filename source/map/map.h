#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

#include "hex_site.h"
#include "map_site.h"

class Map {
   public:
    void draw(sf::RenderTarget& target) const;

    Map_site::ptr<Hex_site>& get_hex(const int& no);
    Map_site::ptr<Hex_site>& get_hex(const int& x, const int& y);
    Map_site::ptr_base& get_site(const int& no);

    void load_map(const std::string& path, const float& size);
    void save_map(const std::string& path);

    static Map create_test_map(const float& size);

    void set_numbers_drawing(const std::string& font_filename);

   private:
    void recompute_geometry(const float& size);
    void resize(const int& x, const int& y);

    std::vector<std::vector<int>> _adjacency_matrix;
    std::vector<Map_site::ptr<Hex_site>> _hexes;
    std::vector<Map_site::ptr_base> _nonhex_sites;

    int _x_dim   = 0;
    int _y_dim   = 0;
    int _n_hexes = 0;

    sf::Font _numbers_font;
    bool _draw_numbers = false;
};