#pragma once

#include <array>
#include <string>

#include "map_site.h"
#include "shapes/hex_shape.h"

#include <cereal/types/array.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <pugixml.hpp>

enum class Hex_type {
    field,
    forest,
    hills,
    mountains,
    swamp
};

std::string type_to_string(const Hex_type &type);
Hex_type string_to_hex_type(const std::string &str);

class Hex_site : public Map_site {
   public:
    explicit Hex_site(const int &hnumber = 0);

    virtual ~Hex_site() = default;

    virtual void draw(sf::RenderTarget &target) const override;
    void draw_number(sf::RenderTarget &target, const sf::Font &font) const;
    virtual Hex_type get_hex_type() const = 0;

    bool contains(const sf::Vector2f &vec) const noexcept;
    void set_highlighted(bool highlighted) noexcept override;
    void set_shape(const float &x, const float &y, const float &radius);

    std::shared_ptr<Map_site> get_side(const Directions &side) const;
    const auto &get_radius() const;
    auto get_small_radius() const;
    const auto &get_position() const;
    Site_type get_site_type() const final;

    void set_side(const Directions &side, std::shared_ptr<Map_site> site);

    void write(pugi::xml_node &node);

   protected:
    virtual void set_color() = 0;

    Hex_shape _shape;

   private:
    std::array<std::shared_ptr<Map_site>, 6> _sides;
    /*
    template <class Archive>
    friend void serialize(Archive &ar, Hex_site &h);
*/
   public:
    template <class Archive>
    void serialize(Archive &ar) {
        ar(cereal::virtual_base_class<Map_site>(this), CEREAL_NVP(_sides));
    }
};
/*
template <class Archive>
void serialize(Archive &ar, Hex_site &h) { ar(cereal::virtual_base_class<Map_site>(&h), CEREAL_NVP(h._sides)); }
*/
inline std::shared_ptr<Map_site> Hex_site::get_side(const Directions &side) const {
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

inline void Hex_site::set_side(const Directions &side, std::shared_ptr<Map_site> site) {
    _sides[static_cast<int>(side)] = site;
}

inline Site_type Hex_site::get_site_type() const {
    return Site_type::hexagon;
}