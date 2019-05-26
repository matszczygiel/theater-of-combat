#pragma once

#include <SFML/Graphics.hpp>

class Hex_shape : public sf::Shape {
   public:
    explicit Hex_shape(const float& radius = 0);

    virtual sf::Vector2f getPoint(std::size_t index) const override;
    virtual std::size_t getPointCount() const override;

    void set_radius(const float& radius);
    float get_small_radius() const;
    const float& get_radius() const;

    static constexpr float get_small_radius(const float& radius);

   private:
    float _radius;
};

inline const float& Hex_shape::get_radius() const { return _radius; }

inline std::size_t Hex_shape::getPointCount() const { return 6; }
