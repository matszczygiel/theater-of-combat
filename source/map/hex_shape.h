#pragma once

#include <SFML/Graphics.hpp>

class Hex_shape : public sf::Shape {
   public:
    explicit Hex_shape(const float& radius = 0);

    void set_radius(const float& radius);
    const float& get_radius() const;
    const float get_small_radius() const;

    inline virtual std::size_t getPointCount() const override { return 6; }

    virtual sf::Vector2f getPoint(std::size_t index) const override;

    static float get_small_radius(const float& radius);

   private:
    float _radius;
};