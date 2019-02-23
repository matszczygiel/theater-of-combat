#include "site_factory.h"

#include "concrete_hex.h"
#include "concrete_passage.h"
#include "log.h"

std::unique_ptr<Hex_site> Map_site_factory::create_hex(const Hex_type& type, const int& number) {
    switch (type) {
        case Hex_type::field:
            return std::make_unique<Field>(number);
        case Hex_type::forest:
            return std::make_unique<Forest>(number);
        default:
            GAME_ERROR("Cannot create hex. Unknown hex type.");
            throw std::domain_error("Cannot create hex. Unknown hex type.");
    }
}

std::unique_ptr<Passage_site> Map_site_factory::create_passage(const Passage_type& type, const int& number) {
    switch (type) {
        case Passage_type::river:
            return std::make_unique<River>(number);
        default:
            GAME_ERROR("Cannot create passage. Unknown passage type.");
            throw std::domain_error("Cannot create passage. Unknown passage type.");
    }
}

std::unique_ptr<Hex_site> Map_site_factory::create_hex(pugi::xml_node& node) {
}
std::unique_ptr<Passage_site> Map_site_factory::create_passage(pugi::xml_node& node) {
}