#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <TGUI/TGUI.hpp>

#include "map/hex_site.h"
#include "tokenizable.h"
#include "unit_type.h"

class Mover;
class Map;
class Hex_site;

class Unit : public Tokenizable {
   public:
    explicit Unit(const int& moving_pts, const int& strength_pts)
        : Tokenizable(), _ocupation(), _moving_pts(moving_pts), _current_moving_pts(moving_pts) {}

    virtual ~Unit() = default;

    virtual std::unique_ptr<Mover> get_mover(std::weak_ptr<Map> map) = 0;

    tgui::Canvas::Ptr create_displayer() const;

    void place_on_hex(std::shared_ptr<Hex_site>& hex);
    void reset_mv_points() noexcept;
    void reduce_mv_points(const int& points);
    void reduce_strength_points(const int& points);

    const auto& get_ocupation() const;
    const auto& get_mv_points() const;

    static void load_font_file(const std::string& filename);

   protected:
    std::shared_ptr<Hex_site> _ocupation = nullptr;

    const int _moving_pts;
    int _current_moving_pts;

   private:
    static sf::Font _font;
};


