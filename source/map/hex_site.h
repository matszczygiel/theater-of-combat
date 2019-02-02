#pragma once

#include "hex_shape.h"
#include "map_site.h"

#include <array>

class Hex_site : public Map_site {
   public:
    explicit Hex_site(const int &hex_number = 0) {
        _highlighted = false;
        _number      = hex_number;
        _sides       = {
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr};
    }

    virtual ~Hex_site() = default;
    virtual void draw(sf::RenderTarget &target) const override;

    bool contains(const sf::Vector2f &vec) const;
    void set_highlighted(bool highlighted) override;
    void set_shape(float x, float y, float radius);

    inline Map_site *get_side(const Directions &side) const override {
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

   protected:
    void set_side_this(const Directions &side, Map_site *site) override;
    virtual void set_color() = 0;

    Hex_shape _shape;

   private:
    std::array<Map_site *, 6> _sides;
};