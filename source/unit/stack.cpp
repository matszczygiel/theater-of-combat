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