#pragma once

#include <string>

#include <SFML/Graphics/RenderTarget.hpp>
#include <pugixml.hpp>

#include "core/registrable.h"

class Map_site : public Registrable<Map_site, std::string, pugi::xml_node> {
   public:
    using id_type          = std::string;
    using registrable_base = Registrable<Map_site, std::string, pugi::xml_node>;

    constexpr explicit Map_site(const int& number) noexcept
        : _number(number) {}

    explicit Map_site(pugi::xml_node& node);

    virtual ~Map_site() = default;

    virtual void draw(sf::RenderTarget& target) const = 0;

    virtual id_type get_name() const = 0;

    virtual void set_highlighted(bool highlighted);
    bool is_highlighted() const noexcept;
    constexpr auto& get_number() const noexcept;

    virtual void serialize(pugi::xml_node& node) const;

    static std::unique_ptr<Map_site> unserialize(pugi::xml_node& node);

    static constexpr auto object_identifier = "map-site";

   protected:
    bool _highlighted = false;

    static constexpr auto type_identifier   = "type";
    static constexpr auto number_identifier = "number";

   private:
    const int _number;
};

#define REGISTER_MAP_SITE(class_name) \
    REGISTER_CLASS(Map_site, class_name, #class_name)

#define DEFINE_MAP_SITE_NAMING(class_name) \
    virtual inline id_type get_name() const override { return #class_name; };

inline bool Map_site::is_highlighted() const noexcept {
    return _highlighted;
}

inline constexpr auto& Map_site::get_number() const noexcept {
    return _number;
}

inline void Map_site::set_highlighted(bool highlighted) {
    _highlighted = highlighted;
}
