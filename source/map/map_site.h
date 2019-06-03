#pragma once

#include <string>

#include <SFML/Graphics/RenderTarget.hpp>

class Map_site {
   public:
    enum class Type {
        Field,
        Forest,

        River
    };

    explicit constexpr Map_site(const int& number = 0) noexcept
        : _number(number) {}

    virtual ~Map_site() = default;

    virtual void draw(sf::RenderTarget& target) const = 0;
    virtual Type get_type() const                     = 0;

    virtual void set_highlighted(bool highlighted);
    bool is_highlighted() const noexcept;
    constexpr const int& get_number() const noexcept;

   private:
    bool _highlighted = false;
    int _number;

   public:
    template <class Archive>
    void serialize(Archive& ar) { ar(_number); }
};

constexpr const int& Map_site::get_number() const noexcept {
    return _number;
}
