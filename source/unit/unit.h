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
    using registrable_base = Registrable<Unit, std::string, pugi::xml_node>;

    explicit Unit(const int& moving_pts, const int& strength_pts,
                  const std::string& description) noexcept;
    explicit Unit(const pugi::xml_node& node);

    virtual ~Unit() = default;

    virtual std::unique_ptr<Mover> get_mover(std::shared_ptr<Map>& map) = 0;
    virtual id_type get_name() const                                   = 0;

    tgui::Canvas::Ptr create_displayer() const;

    void place_on_hex(std::shared_ptr<Hex_site>& hex);
    void reset_mv_points() noexcept;
    void reduce_mv_points(const int& points);
    void reduce_st_points(const int& points);

    const std::shared_ptr<Hex_site>& get_occupation() const;
    const int& get_mv_points() const;
    const int& get_st_points() const;

    static void load_font_file(const std::string& filename);

    virtual void serialize(pugi::xml_node& node) const;
    static std::unique_ptr<Unit> unserialize(pugi::xml_node& node);

   protected:
    static constexpr auto obj_identifier    = "unit";
    static constexpr auto type_identifier   = "type";

   private:
    std::shared_ptr<Hex_site> _occupation = nullptr;

    const int _moving_pts;
    int _current_moving_pts;
    int _strength_pts;

    std::string _description;

    static sf::Font _font;
};

#define REGISTER_UNIT(class_name) \
    REGISTER_CLASS(Unit, class_name, #class_name)

#define DEFINE_UNIT_NAMING(class_name) \
    virtual inline id_type get_name() const override { return #class_name; };
