#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

#include "hex_site.h"
#include "map_site.h"
#include "passage_site.h"

class Map {
   public:
    void draw(sf::RenderTarget& target) const;

    std::shared_ptr<Hex_site> get_hex(const int& no);
    std::shared_ptr<Hex_site> get_hex(const int& x, const int& y);
    std::shared_ptr<Map_site>& get_site(const int& no);

    //   void load_map(const std::string& path, const float& size);
    //   void save_map(const std::string& path);

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

    std::vector<std::vector<int>> _adjacency_matrix;
    std::vector<std::shared_ptr<Map_site>> _sites;
    std::vector<std::shared_ptr<Hex_site>> _hexes;
    std::vector<std::shared_ptr<Passage_site>> _passages;

    int _x_dim   = 0;
    int _y_dim   = 0;
    int _n_hexes = 0;

    sf::Font _numbers_font;
    bool _draw_numbers = false;
};

inline std::shared_ptr<Map_site>& Map::get_site(const int& no) {
    return _sites.at(no);
}

template <class T>
void Map::connect_site(const int& hex1_no, const int& hex2_no) {
    static_assert(std::is_base_of<Passage_site, T>::value, "Trying to connect non passage site.");
    const int no = _adjacency_matrix.size();
    _sites.push_back(std::make_unique<T>(no));
    const auto no1 = res.get_hex(5, i)->get_number();
    const auto no2 = res.get_hex(6, i)->get_number();

    res._adjacency_matrix.emplace_back({no1, no2});
    std::replace(res._adjacency_matrix.at(no1).begin(), res._adjacency_matrix.at(no1).end(), no2, no);
    std::replace(res._adjacency_matrix.at(no2).begin(), res._adjacency_matrix.at(no2).end(), no1, no);
}
