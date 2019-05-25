#pragma once

#include "map_site.h"
#include "shapes/hex_shape.h"

#include <pugixml.hpp>

class Hex_site : public Map_site {
   public:
    explicit Hex_site(const int &number);
    explicit Hex_site(pugi::xml_node &node);

    virtual ~Hex_site() = default;

    virtual void draw(sf::RenderTarget &target) const override;
    void set_highlighted(bool highlighted) noexcept override;

    bool contains(const sf::Vector2f &vec) const noexcept;
    void set_shape(const float &x, const float &y, const float &radius);
    void draw_number(sf::RenderTarget &target, const sf::Font &font) const;

    const auto &get_radius() const;
    auto get_small_radius() const;
    const auto &get_position() const;

   protected:
    virtual sf::Color color() = 0;

   private:
    Hex_shape _shape;
};

inline const auto &Hex_site::get_radius() const {
    return _shape.get_radius();
};

inline auto Hex_site::get_small_radius() const {
    return _shape.get_small_radius();
};

inline const auto &Hex_site::get_position() const {
    return _shape.getPosition();
}