#include "unit.h"

#include "log.h"

void Unit::draw(sf::RenderTarget& window) {
    window.draw(_token);
}

void Unit::init_token(const float& size) {
    _token.setSize(sf::Vector2f(size, size));
    set_color();
    _token.setOutlineThickness(-0.05 * size);
    _token.setOutlineColor(sf::Color::Black);
    GAME_TRACE("Unit token intialized of size: {0}.", size);
}

void Unit::place_on_hex(Hex_site* hex) {
    _ocupation = hex;
    _token.setPosition(_ocupation->get_position().x - 0.5 * _token.getSize().x,
                       _ocupation->get_position().y - 0.5 * _token.getSize().y);
    GAME_INFO("Unit placed on hex No. {0} at ({1}, {2})",
              _ocupation->get_number(),
              _ocupation->get_position().x,
              _ocupation->get_position().y);
}

void Unit::reset_mv_points() noexcept {
    GAME_TRACE("Reseting moving points.");
    _current_moving_pts = _moving_pts;
}

void Unit::reduce_mv_points(const int& points) {
    if (points > _current_moving_pts)
        throw std::domain_error("Trying to reduce mv points to negative value.");

    _current_moving_pts -= points;
}