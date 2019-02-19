#pragma once

#include "hex_shape.h"
#include "map_site.h"

#include <array>

class Hex_site : public Map_site {
   public:
    explicit Hex_site(const int &hnumber = 0);

    virtual ~Hex_site() = default;

    virtual void draw(sf::RenderTarget &target) const override;

    bool contains(const sf::Vector2f &vec) const noexcept;
    void set_highlighted(bool highlighted) noexcept override;
    void set_shape(const float &x, const float &y, const float &radius);

    Map_site *get_side(const Directions &side) const;
    const auto &get_radius() const;
    auto get_small_radius() const;
    const auto &get_position() const;

    void set_side(const Directions &side, Map_site *site);

   protected:
    virtual void set_color() = 0;

    Hex_shape _shape;

   private:
    std::array<Map_site *, 6> _sides;
};

inline Map_site *Hex_site::get_side(const Directions &side) const {
    return _sides[static_cast<int>(side)];
}

inline const auto &Hex_site::get_radius() const {
    return _shape.get_radius();
};

inline auto Hex_site::get_small_radius() const {
    return _shape.get_small_radius();
};

inline const auto &Hex_site::get_position() const {
    return _shape.getPosition();
}

inline void Hex_site::set_side(const Directions &side, Map_site *site) {
    _sides[static_cast<int>(side)] = site;
}