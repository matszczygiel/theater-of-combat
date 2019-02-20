#pragma once

#include <SFML/Graphics.hpp>

class Hex_border_shape : public sf::Shape {
   public:
    Hex_border_shape() = default;
    explicit Hex_border_shape(const float& length, const float& width_ratio = 0.1);

    virtual sf::Vector2f getPoint(std::size_t index) const override;
    virtual std::size_t getPointCount() const override;

    void set_length(const float& length);

   private:
    float _length;
    float _width_ratio;
};

inline std::size_t Hex_border_shape::getPointCount() const { return 4; }
