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
    auto box = tgui::BitmapButton::create();
    //sf::RenderTexture rt;
    //rt.create(_token.getLocalBounds().width, _token.getLocalBounds().height);
    //rt.draw(_token);
    //
    //box->setImage(rt.getTexture());
    //box->setText("unit");
    //target->add(box, name);
}