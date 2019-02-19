#include "passage_site.h"
#include "log.h"

Passage_site::Passage_site(const int &number)
    : Map_site(number), _shape(), _sides{nullptr, nullptr}, _sides_directions() {}

void Passage_site::set_sides(const Directions &side1, Hex_site *site1,
                             const Directions &side2, Hex_site *site2) {
    if (side1 != Map_site::opposite_direction(side2)) {
        GAME_CRITICAL("Passage site initialized with not oposite sites");
        throw("Passage site initialized with not oposite sites");
    }

    _sides            = {site1, site2};
    _sides_directions = {side1, side2};

    site1->set_side(side2, this);
    site2->set_side(side1, this);

    set_shape();
}

void Passage_site::unstage_sides() {
    std::get<0>(_sides)->set_side(std::get<1>(_sides_directions), static_cast<Map_site *>(std::get<1>(_sides)));
    std::get<1>(_sides)->set_side(std::get<0>(_sides_directions), static_cast<Map_site *>(std::get<0>(_sides)));

    _sides = {nullptr, nullptr};
}

Hex_site *Passage_site::other_side(Hex_site *adjacet_site) {
    if (std::get<0>(_sides) == adjacet_site)
        return std::get<1>(_sides);
    else if (std::get<1>(_sides) == adjacet_site)
        return std::get<0>(_sides);
    else {
        GAME_ERROR("Passage {0} asked for other side of not adjacent site {1}", _number, adjacet_site->get_number());
        throw std::domain_error("Passage asked for other side of not adjacent site.");
    }
}

void Passage_site::draw(sf::RenderTarget &target) const {
    target.draw(_shape);
}

void Passage_site::set_highlighted(bool highlighted) noexcept {
    _highlighted = highlighted;
}
