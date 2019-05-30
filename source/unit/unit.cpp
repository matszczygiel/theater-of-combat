#include "unit.h"

#include "log.h"

sf::Font Unit::_font;

const auto& Unit::get_ocupation() const { return _ocupation; }

const auto& Unit::get_mv_points() const { return _current_moving_pts; }

void Unit::place_on_hex(Hex_site* hex) {
    _ocupation = hex;
    Tokenizable::set_token_postion(_ocupation->get_position());
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
    assert(points <= _current_moving_pts);

    _current_moving_pts -= points;
}

void Unit::reduce_strength_points(const int& points) {
    assert(points <= _strength_pts);

    _strength_pts -= points;
}

tgui::Canvas::Ptr Unit::create_displayer() const {
    auto canvas = tgui::Canvas::create({150, 80});
    canvas->setPosition(10, 10);
    canvas->clear(sf::Color::Green);
    auto token = get_token();
    token.setPosition(0, 0);
    canvas->draw(token);

    sf::Text moving_info("Moving points:    " + std::to_string(_current_moving_pts), _font, 10);
    sf::Text strength_info("Strength points: " + std::to_string(_strength_pts), _font, 10);

    moving_info.setPosition(35, 10);
    strength_info.setPosition(35, 25);

    moving_info.setFillColor(sf::Color::Red);
    strength_info.setFillColor(sf::Color::Red);

    canvas->draw(moving_info);
    canvas->draw(strength_info);

    canvas->display();
    return canvas;
}

void Unit::load_font_file(const std::string& filename) {
    _font.loadFromFile(filename);
    GAME_INFO("Loading unit info font.");
}