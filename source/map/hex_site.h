#pragma once

#include "map_site.h"
#include "shapes/hex_shape.h"

#include <cereal/types/array.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

class Hex_site : public Map_site {
   public:
    explicit Hex_site(const int &number);

    virtual ~Hex_site() = default;

    virtual void draw(sf::RenderTarget &target) const override;
    void set_highlighted(bool highlighted) noexcept override;

    bool contains(const sf::Vector2f &vec) const noexcept;
    void set_shape(const float &x, const float &y, const float &radius);
    void draw_number(sf::RenderTarget &target, const sf::Font &font) const;

    const float &get_radius() const;
    float get_small_radius() const;
    const sf::Vector2f &get_position() const;

   protected:
    virtual sf::Color color() = 0;

   private:
    Hex_shape _shape;

};
