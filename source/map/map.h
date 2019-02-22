#pragma once

#include <memory>
#include <vector>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>

#include "hex_site.h"
#include "passage_site.h"

class Map {
   public:
    using Hex_matrix = std::vector<std::vector<std::unique_ptr<Hex_site>>>;
    using Passage_vec = std::vector<std::unique_ptr<Passage_site>>;

    void draw(sf::RenderTarget& target) const;
    Hex_site* get_hex(const int& no);
    Passage_site* get_passage(const int& no);

    void load_map(const std::string& path);
    void save_map(const std::string& path);

    static Map create_test_map(const float& size);

   private:
    Hex_matrix _map;
    Passage_vec _passages;
};