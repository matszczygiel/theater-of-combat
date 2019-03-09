#include "stack.h"

void Stack::draw(sf::RenderTarget& target) const {
    Tokenizable::draw(target);

    if (_display_content) {
        sf::Vector2f start = get_token_position();
        start.y += 1.5 * get_token_size();
        start.x += (_stack.size() - 1) / 2.f * 1.5 * get_token_size();

        sf::Vector2f displacement(1.5 * get_token_size(), 0);

        for (auto& u : _stack) {
            auto org_pos = u->get_token_position();
            u->set_token_postion(start);
            u->draw(target);
            u->set_token_postion(org_pos);
            start += displacement;
        }
    }
}

tgui::Grid::Ptr Stack::create_displayer(const std::function<void(Unit*)>& widget_callback) const {
    auto grid = tgui::Grid::create();
    grid->setPosition(10, 10);
    int counter = 0;
    for (auto& u : _stack) {
        auto disp = u->create_displayer();
        disp->connect("Clicked", widget_callback, u);
        grid->addWidget(disp, counter++, 0, tgui::Padding{0, 0, 0, 10});
    }
    return grid;
}