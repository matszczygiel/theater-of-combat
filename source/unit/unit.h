#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <TGUI/TGUI.hpp>

#include <pugixml.hpp>

#include "core/registrable.h"
#include "map/hex_site.h"
#include "tokenizable.h"

class Mover;
class Map;
class Hex_site;

class Unit : public Tokenizable, public Registrable<Unit, std::string, pugi::xml_node> {
   public:
    using id_type          = std::string;
    using registrable_base = Registrable<Map_site, std::string, pugi::xml_node>;

    explicit Unit(const int& moving_pts, const int& strength_pts) noexcept;
    explicit Unit(const pugi::xml_node& node);

    virtual ~Unit() = default;

    virtual std::unique_ptr<Mover> get_mover(std::shared_ptr<Map> map) = 0;
    virtual id_type get_name() const                                   = 0;

    tgui::Canvas::Ptr create_displayer() const;

    void place_on_hex(std::shared_ptr<Hex_site>& hex);
    void reset_mv_points() noexcept;
    void reduce_mv_points(const int& points);
    void reduce_strength_points(const int& points);

    const auto& get_occupation() const;
    const auto& get_mv_points() const;

    static void load_font_file(const std::string& filename);

   protected:
    std::shared_ptr<Hex_site> _occupation = nullptr;

    const int _moving_pts;
    int _current_moving_pts;

   private:
    static sf::Font _font;
};

#define REGISTER_UNIT(class_name) \
    REGISTER_CLASS(Unit, class_name, #class_name)

#define DEFINE_UNIT_NAMING(class_name) \
    virtual inline id_type get_name() const override { return #class_name; };
