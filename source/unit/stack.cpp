#include "stack.h"

void Stack::draw(sf::RenderTarget& window) {
    window.draw(_token);

    if (_display_content) {
        sf::Vector2f start = _token.getPosition();
        start.y += 1.5 * _token.getSize().x;
        start.x += (_stack.size() - 1) / 2.f * 1.5 * _token.getSize().x;

        sf::Vector2f displacement(1.5 * _token.getSize().x, 0);
        
        for (auto& u : _stack) {
            u->draw_token_at(window, start);
            start += displacement;
        }
    }
}