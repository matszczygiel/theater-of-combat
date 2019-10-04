#ifndef HEXAGONS_H
#define HEXAGONS_H
// Classes provinding hexagonal grid functionality based on:
// https://www.redblobgames.com/grids/hexagons/

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

#include <SFML/System/Vector2.hpp>
#include <cereal/types/utility.hpp>

#include "core/log.h"

template <typename T>
class HexCoordinates {
    static_assert(std::is_arithmetic<T>::value,
                  "HexCoordinates is parametrized by arithmetic types.");

   public:
    constexpr HexCoordinates() noexcept : _x(0), _y(0), _z(0) {}
    constexpr HexCoordinates(T q, T p) noexcept : _x{q}, _y{-p - q}, _z{p} {}
    HexCoordinates(T x, T y, T z);

    constexpr HexCoordinates<T> neighbor(int direction) const;
    constexpr std::array<HexCoordinates<T>, 6> neighbors() const;

    constexpr const T& p() const { return _z; }
    constexpr const T& q() const { return _x; }

    constexpr const T& x() const { return _x; }
    constexpr const T& y() const { return _y; }
    constexpr const T& z() const { return _z; }

    constexpr T length() const;

    constexpr static HexCoordinates<T> origin();

    template <class Archive>
    void serialize(Archive& archive);

   private:
    T _x;
    T _y;
    T _z;

    static constexpr std::array<HexCoordinates<T>, 6> directions;
};
using HexCoordinate           = HexCoordinates<int>;
using HexCoordinateFractional = HexCoordinates<float>;

template <typename T>
HexCoordinates<T>::HexCoordinates(T x, T y, T z) : _x{x}, _y{y}, _z{z} {
    engine_assert_throw(x + y + z == 0, "HexCoordinates x + y + z must equal 0.");
}

template <typename T>
constexpr HexCoordinates<T> HexCoordinates<T>::origin() {
    return HexCoordinates<T>();
}

template <typename T>
const std::array<HexCoordinates<T>, 6> HexCoordinates<T>::directions = {
    HexCoordinates<T>(1, -1, 0), HexCoordinates<T>(1, 0, -1), HexCoordinates<T>(0, 1, -1),
    HexCoordinates<T>(-1, 1, 0), HexCoordinates<T>(-1, 0, 1), HexCoordinates<T>(0, -1, 1),
};

template <typename T>
constexpr T HexCoordinates<T>::length() const {
    return (std::abs(_x) + std::abs(_y) + std::abs(_z)) / 2.0;
}

template <typename T>
constexpr HexCoordinates<T> HexCoordinates<T>::neighbor(int direction) const {
    return *this + directions.at(direction);
}

template <typename T>
constexpr std::array<HexCoordinates<T>, 6> HexCoordinates<T>::neighbors() const {
    std::array<HexCoordinates<T>, 6> res;
    std::generate(res.begin(), res.end(),
                  [i = 0, this]() mutable { return neighbor(i++); });
    return res;
}

HexCoordinate round(const HexCoordinateFractional& hex);

template <typename T, typename U>
constexpr typename std::common_type<T, U>::type distance(const HexCoordinates<T>& lhs,
                                                         const HexCoordinates<U>& rhs) {
    return (lhs - rhs).length();
}

template <typename T>
constexpr bool operator==(const HexCoordinates<T>& lhs, const HexCoordinates<T>& rhs) {
    return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
}

template <typename T>
constexpr bool operator!=(const HexCoordinates<T>& lhs, const HexCoordinates<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
constexpr HexCoordinates<typename std::common_type<T, U>::type> operator+(
    const HexCoordinates<T>& lhs, const HexCoordinates<U>& rhs) {
    return HexCoordinates<typename std::common_type<T, U>::type>(
        lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

template <typename T, typename U>
constexpr HexCoordinates<typename std::common_type<T, U>::type> operator-(
    const HexCoordinates<T>& lhs, const HexCoordinates<U>& rhs) {
    return HexCoordinates<typename std::common_type<T, U>::type>(
        lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

template <typename T, typename U>
constexpr HexCoordinates<typename std::common_type<T, U>::type> operator*(
    const T& lhs, const HexCoordinates<U>& rhs) {
    return HexCoordinates<typename std::common_type<T, U>::type>(
        lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
}

template <typename T, typename U>
constexpr HexCoordinates<typename std::common_type<T, U>::type> operator*(
    const HexCoordinates<T>& lhs, const U& rhs) {
    return rhs * lhs;
}

template <typename T>
template <class Archive>
void HexCoordinates<T>::serialize(Archive& archive) {
    archive(CEREAL_NVP(_x), CEREAL_NVP(_y), CEREAL_NVP(_z));
}

struct Orientation {
   public:
    Orientation() = delete;
    // orientation matrix (used in the conversion to pixel point), row major
    std::array<float, 4> m{};
    std::array<float, 4> minv{};
    float start_angle{};

   public:
    static const Orientation Pointy;
    static const Orientation Flat;
};

struct Layout {
    Orientation orientation{Orientation::Pointy};
    sf::Vector2f size{50.0, 50.0};
    sf::Vector2f origin{0.0, 0.0};

    sf::Vector2f cornerr_offset(int corner) const;
};

sf::Vector2f hex_to_world_point(const HexCoordinate& hex, const Layout& layout);
HexCoordinate world_point_to_hex(sf::Vector2f point, const Layout& layout);

#endif