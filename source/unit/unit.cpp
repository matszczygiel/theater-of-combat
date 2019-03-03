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
    assert(points <= _current_moving_pts);

    _current_moving_pts -= points;
}

void Unit::draw_token_at(sf::RenderTarget& window, const sf::Vector2f& position) const {
    sf::RectangleShape token_copy = _token;
    token_copy.setPosition(position);
    window.draw(token_copy);
}

void Unit::create_displayer(tgui::Panel::Ptr& target, const std::string& name) const {
    auto box = tgui::BitmapButton::create();
    sf::RenderTexture rt;
    rt.create(_token.getLocalBounds().width, _token.getLocalBounds().height);
    rt.draw(_token);
    
    box->setImage(rt.getTexture());
    box->setText("unit");
    target->add(box, name);
}