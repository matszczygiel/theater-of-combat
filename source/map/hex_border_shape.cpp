#include "hex_border_shape.h"

Hex_border_shape::Hex_border_shape(const float& length, const float& width_ratio) 
: _length(length) , _width_ratio(width_ratio) {
    update();
}

sf::Vector2f Hex_border_shape::getPoint(std::size_t index) const {
    const float x = _length / 2.f;
    const float y = x * _width_ratio;
    switch (index) {
        case 0:
            return sf::Vector2f(-x, -y);
        case 1:
            return sf::Vector2f(x, -y);
        case 2:
            return sf::Vector2f(x, y);
        case 3:
            return sf::Vector2f(-x, y);
        default:
            return sf::Vector2f(0, 0);
    }
}

void Hex_border_shape::set_length(const float& length) {
    _length = length;
    update();
}
