#pragma once

#include <memory>

#include <pugixml.hpp>

#include "hex_site.h"
#include "passage_site.h"

class Map_site_factory {
   public:
    Map_site_factory() = delete;

    static std::unique_ptr<Hex_site> create_hex(const Hex_type& type, const int& number = 0);
    static std::unique_ptr<Passage_site> create_passage(const Passage_type& type, const int& number = 0);

    static std::unique_ptr<Hex_site> create_hex(pugi::xml_node& node);
    static std::unique_ptr<Passage_site> create_passage(pugi::xml_node& node);

};
