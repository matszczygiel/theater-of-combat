#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

#include "hex_site.h"
#include "passage_site.h"

#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

class Map {
   public:
    using Hex_matrix  = std::vector<std::vector<std::shared_ptr<Hex_site>>>;
    using Passage_vec = std::vector<std::shared_ptr<Passage_site>>;

    void draw(sf::RenderTarget& target) const;
    std::shared_ptr<Hex_site>& get_hex(const int& no);
    std::shared_ptr<Hex_site>& get_hex(const int& x, const int& y);
    std::shared_ptr<Passage_site>& get_passage(const int& no);

    void load_map(const std::string& path, const float& size);
    void save_map(const std::string& path);

    static Map create_test_map(const float& size);

    void set_numbers_drawing(const std::string& font_filename);

   private:
    void recompute_geometry(const float& size);
    void resize(const int& x, const int& y);

    Hex_matrix _map;
    Passage_vec _passages;

    int _x_dim;
    int _y_dim;

    sf::Font _numbers_font;
    bool _draw_numbers = false;

   public:
    template <class Archive>
    void serialize(Archive& ar) { ar(_x_dim, _y_dim, _map, _passages); }
};