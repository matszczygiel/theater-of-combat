#include "unit.h"

#include "log.h"

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

void Unit::create_displayer(tgui::Panel::Ptr& target, const std::string& name) const {
    auto canvas = tgui::Canvas::create({"80%", "10%"});
    canvas->setPosition({"10%", "10%"});

    canvas->clear();
    auto token = get_token();
    token.setPosition(0, 0);
    canvas->draw(token);
    canvas->display();
    target->add(canvas, name);
}