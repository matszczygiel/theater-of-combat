#include "unit.h"

#include "log.h"

sf::Font Unit::_font;

Unit::Unit(const int& moving_pts, const int& strength_pts,
           const std::string& description) noexcept
    : _moving_pts(moving_pts),
      _current_moving_pts(moving_pts),
      _strength_pts(strength_pts),
      _description(description) {}

Unit::Unit(const pugi::xml_node& node)
    : _moving_pts(node.attribute("mv_pts").as_int()),
      _current_moving_pts(_moving_pts),
      _strength_pts(node.attribute("st_pts").as_int()),
      _description(node.attribute("description").as_string()) {}

const std::shared_ptr<Hex_site>& Unit::get_occupation() const { return _occupation; }

const int& Unit::get_mv_points() const { return _current_moving_pts; }

const int& Unit::get_st_points() const { return _strength_pts; }

void Unit::place_on_hex(std::shared_ptr<Hex_site>& hex) {
    _occupation = hex;
    Tokenizable::set_token_postion(_occupation->get_position());
    GAME_INFO("Unit placed on hex No. {0} at ({1}, {2})",
              _occupation->get_number(),
              _occupation->get_position().x,
              _occupation->get_position().y);
}

void Unit::reset_mv_points() noexcept {
    ENGINE_TRACE("Reseting moving points.");
    _current_moving_pts = _moving_pts;
}

void Unit::reduce_mv_points(const int& points) {
    assert(points <= _current_moving_pts);

    _current_moving_pts -= points;
}

void Unit::reduce_st_points(const int& points) {
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

std::unique_ptr<Unit> Unit::unserialize(pugi::xml_node& node) {
    const id_type type = node.attribute("type").value();
    return registrable_base::create(type, node);
}

void Unit::serialize(pugi::xml_node& node) const {
    auto child = node.append_child(obj_identifier);
    child.append_attribute(type_identifier).set_value(get_name().c_str());
    child.append_attribute("mv_pts").set_value(_moving_pts);
    child.append_attribute("st_pts").set_value(_strength_pts);
    child.append_attribute("description").set_value(_description.c_str());
}