#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <TGUI/TGUI.hpp>

#include "map/hex_site.h"
#include "tokenizable.h"
#include "unit_type.h"

class Mover;

class Unit : public Tokenizable {
   public:
    explicit Unit(const int& moving_pts)
        : Tokenizable(), _ocupation(), _moving_pts(moving_pts), _current_moving_pts(moving_pts) {}

    virtual ~Unit() noexcept = default;

    virtual Unit_type get_type() const = 0;
    virtual Mover* get_mover()         = 0;

    void create_displayer(tgui::Panel::Ptr& target, const std::string& name) const;

    void place_on_hex(Hex_site* hex);
    void reset_mv_points() noexcept;
    void reduce_mv_points(const int& points);

    const auto& get_ocupation() const;
    const auto& get_mv_points() const;

   protected:
    Hex_site* _ocupation;

    const int _moving_pts;
    int _current_moving_pts;
};

inline const auto& Unit::get_ocupation() const { return _ocupation; }

inline const auto& Unit::get_mv_points() const { return _current_moving_pts; }

