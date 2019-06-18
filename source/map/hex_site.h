#pragma once

#include "map_site.h"
#include "shapes/hex_shape.h"

#include <cereal/types/base_class.hpp>

class Hex_site : public Map_site {
   public:
    enum class Type {
        Field,
        Forest
    };

    explicit Hex_site(const int &number, const Type &type = Type::Field);
    Hex_site()  = default;
    ~Hex_site() = default;

    void draw(sf::RenderTarget &target) const final;
    void set_highlighted(bool highlighted) noexcept final;

    bool contains(const sf::Vector2f &vec) const noexcept;
    void set_shape(const sf::Vector2f &position, const float &radius);
    void draw_number(sf::RenderTarget &target, const sf::Font &font) const;
    void set_type(const Type &type);
    const Type &get_type() const;

    const float &get_radius() const;
    float get_small_radius() const;
    const sf::Vector2f &get_position() const;

    template <class Archive>
    void save(Archive &ar) const;

    template <class Archive>
    void load(Archive &ar);

   private:
    Hex_shape _shape{};
    Type _type{Type::Field};
};

template <class Archive>
void Hex_site::save(Archive &ar) const {
    ar(cereal::virtual_base_class<Map_site>(this), _type);
}

template <class Archive>
void Hex_site::load(Archive &ar) {
    ar(cereal::virtual_base_class<Map_site>(this), _type);
    set_type(_type);
}
CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(Hex_site, cereal::specialization::member_load_save);