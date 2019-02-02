#pragma once

#include "hex_shape.h"
#include "map_site.h"

#include <array>

class Hex_site : public Map_site {
   public:
    Hex_site(const int& hex_number = 0) {
        _highlighted = false;
        _hex_number  = hex_number;
        _sides       = {
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr};
    }

    virtual void set_shape(float x, float y, float radius);

    virtual void draw(sf::RenderTarget& target) const override;

    inline Map_site* get_side(const Directions& side) const override {
        return _sides[static_cast<int>(side)];
    }

    inline auto get_radius() const {
        return _shape.get_radius();
    };

    inline auto get_small_radius() const {
        return _shape.get_small_radius();
    };

    inline auto get_position() const {
        return _shape.getPosition();
    }

    bool contains(const sf::Vector2f& vec) const;

    void set_highlighted(bool highlighted) override;

    virtual Site_type get_type() const override { return Site_type::none; }

   protected:
    void set_side_this(const Directions& side, Map_site* site) override;

   private:
    std::array<Map_site*, 6> _sides;
    Hex_shape _shape;
};