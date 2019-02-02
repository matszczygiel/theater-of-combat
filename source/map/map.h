#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

#include "hex_site.h"

class Map {
   public:
    using Hex_matrix = std::vector<std::vector<std::unique_ptr<Hex_site>>>;

    void draw(sf::RenderTarget& target) const;
    Hex_site* get_hex(const int& no);

    static Map create_test_map(const float& size);

   private:
    Hex_matrix _map;
};