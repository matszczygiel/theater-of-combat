#pragma once

#include <memory>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <TGUI/TGUI.hpp>

#include "tokenizable.h"

class Mover;
class Map;
class Hex_site;

class Unit : public Tokenizable, std::enable_shared_from_this<Unit> {
   public:
    explicit Unit(const int& moving_pts, const int& strength_pts,
                  const std::string& description) noexcept;

    virtual ~Unit() = default;

    virtual std::unique_ptr<Mover> get_mover(std::shared_ptr<Map>& map) = 0;

    tgui::Canvas::Ptr create_displayer() const;

    void place_on_hex(std::shared_ptr<Hex_site>& hex);
    void reset_mv_points() noexcept;
    void reduce_mv_points(const int& points);
    void reduce_st_points(const int& points);

    const std::shared_ptr<Hex_site>& get_occupation() const;
    const int& get_mv_points() const;
    const int& get_st_points() const;

    static void load_font_file(const std::string& filename);

   private:
    std::shared_ptr<Hex_site> _occupation = nullptr;

    const int _moving_pts;
    int _current_moving_pts;
    int _strength_pts;

    std::string _description;

    static sf::Font _font;

   public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(_moving_pts, _current_moving_pts, _strength_pts, _description);
    }
};
