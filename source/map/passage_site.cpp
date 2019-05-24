#include "passage_site.h"
#include "log.h"

std::string type_to_string(const Passage_type &type) {
    switch (type) {
        case Passage_type::stream:
            return "stream";
        case Passage_type::creek:
            return "creek";
        case Passage_type::river:
            return "river";
        case Passage_type::huge_river:
            return "huge_river";
        case Passage_type::road:
            return "road";
        default:
            GAME_WARN("Invalid passage type to string conversion.");
            return "";
    }
}

Passage_type string_to_passage_type(const std::string &str) {
    if (str == "stream")
        return Passage_type::stream;
    else if (str == "creek")
        return Passage_type::creek;
    else if (str == "river")
        return Passage_type::river;
    else if (str == "huge_river")
        return Passage_type::huge_river;
    else if (str == "road")
        return Passage_type::road;
    else {
        GAME_ERROR("Invalid string to passage type conversion.");
        throw std::runtime_error("Invalid string to passage type conversion.");
    }
}

Passage_site::Passage_site(const int &number)
    : Map_site(number), _sides{nullptr, nullptr}, _sides_directions() {}

void Passage_site::set_sides(const Directions &side1, std::shared_ptr<Hex_site> site1,
                             const Directions &side2, std::shared_ptr<Hex_site> site2) {
    if (side1 != Map_site::opposite_direction(side2)) {
        GAME_CRITICAL("Passage site initialized with not oposite sites");
        throw("Passage site initialized with not oposite sites");
    }

    _sides            = {site1, site2};
    _sides_directions = {side1, side2};

    site1->set_side(side2, shared_from_this());
    site2->set_side(side1, shared_from_this());

    set_shape();
}

void Passage_site::unstage_sides() {
    std::get<0>(_sides)->set_side(std::get<1>(_sides_directions), std::static_pointer_cast<Map_site> (std::get<1>(_sides)));
    std::get<1>(_sides)->set_side(std::get<0>(_sides_directions), std::static_pointer_cast<Map_site> (std::get<0>(_sides)));

    _sides = {nullptr, nullptr};
}

std::shared_ptr<Hex_site> Passage_site::other_side(std::shared_ptr<Hex_site> adjacet_site) {
    if (std::get<0>(_sides) == adjacet_site)
        return std::get<1>(_sides);
    else if (std::get<1>(_sides) == adjacet_site)
        return std::get<0>(_sides);
    else {
        GAME_ERROR("Passage {0} asked for other side of not adjacent site {1}", _number, adjacet_site->get_number());
        throw std::domain_error("Passage asked for other side of not adjacent site.");
    }
}

void Passage_site::set_highlighted(bool highlighted) noexcept {
    _highlighted = highlighted;
}

void Passage_site::write(pugi::xml_node &node) {
    auto pass_node = node.append_child("passage");

    pass_node.append_attribute("number").set_value(_number);
    pass_node.append_attribute("passage_type").set_value(type_to_string(get_passage_type()).c_str());

    const auto &side0     = std::get<0>(_sides);
    const auto &side1     = std::get<1>(_sides);
    const auto &side0_dir = std::get<0>(_sides_directions);
    const auto &side1_dir = std::get<1>(_sides_directions);

    auto sides_node = pass_node.append_child("sides_array");

    if (side0 != nullptr) {
        auto single_side_node0 = sides_node.append_child("side");
        single_side_node0.append_attribute("direction").set_value(direction_to_string(side0_dir).c_str());
        std::static_pointer_cast<Map_site> (side0)->write(single_side_node0);
    }

    if (side1 != nullptr) {
        auto single_side_node1 = sides_node.append_child("side");
        single_side_node1.append_attribute("direction").set_value(direction_to_string(side1_dir).c_str());
        std::static_pointer_cast<Map_site> (side1)->write(single_side_node1);
    }
}