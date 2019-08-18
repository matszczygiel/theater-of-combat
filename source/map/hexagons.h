#ifndef HEXAGONS_H
#define HEXAGONS_H
// Classes provinding hexagonal grid functionality based on:
// https://www.redblobgames.com/grids/hexagons/

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

#include <SFML/System/Vector2.hpp>

template <typename T>
class HexCoordinates {
    static_assert(std::is_arithmetic<T>::value,
                  "HexCoordinates is parametrized by arithmetic types.");

   private:
    T _x;
    T _y;
    T _z;

    static const std::array<HexCoordinates<T>, 6> directions;

   public:
    constexpr HexCoordinates() noexcept : _x(0), _y(0), _z(0) {}

    HexCoordinates(T x, T y, T z) : _x{x}, _y{y}, _z{z} {
        if (x + y + z != 0)
            throw std::domain_error("Sum of x + y + z must be equal 0.");
    }

    constexpr HexCoordinates(T q, T p) noexcept : _x{q}, _y{-p - q}, _z{p} {}

    HexCoordinates<T> neighbor(int direction) const;
    std::array<HexCoordinates<T>, 6> neighbors() const;

    const T &p() const { return _z; }
    const T &q() const { return _x; }

    const T &x() const { return _x; }
    const T &y() const { return _y; }
    const T &z() const { return _z; }

    T length() const;

    static HexCoordinates<T> origin() { return HexCoordinates<T>(); }
};
using HexCoordinate           = HexCoordinates<int>;
using HexCoordinateFractional = HexCoordinates<float>;

template <typename T>
const std::array<HexCoordinates<T>, 6> HexCoordinates<T>::directions = {
    HexCoordinates<T>(1, -1, 0), HexCoordinates<T>(1, 0, -1),
    HexCoordinates<T>(0, 1, -1), HexCoordinates<T>(-1, 1, 0),
    HexCoordinates<T>(-1, 0, 1), HexCoordinates<T>(0, -1, 1),
};

HexCoordinate round(const HexCoordinateFractional &hex);

template <typename T>
T HexCoordinates<T>::length() const {
    return (std::abs(_x) + std::abs(_y) + std::abs(_z)) / 2.0;
}

template <typename T, typename U>
typename std::common_type<T, U>::type distance(const HexCoordinates<T> &lhs,
                                               const HexCoordinates<U> &rhs) {
    return (lhs - rhs).length();
}

template <typename T, typename U>
bool operator==(const HexCoordinates<T> &lhs, const HexCoordinates<U> &rhs) {
    return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
}

template <typename T, typename U>
bool operator!=(const HexCoordinates<T> &lhs, const HexCoordinates<U> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
HexCoordinates<typename std::common_type<T, U>::type> operator+(
    const HexCoordinates<T> &lhs, const HexCoordinates<U> &rhs) {
    return HexCoordinates<typename std::common_type<T, U>::type>(
        lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

template <typename T, typename U>
HexCoordinates<typename std::common_type<T, U>::type> operator-(
    const HexCoordinates<T> &lhs, const HexCoordinates<U> &rhs) {
    return HexCoordinates<typename std::common_type<T, U>::type>(
        lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

template <typename T>
HexCoordinates<T> HexCoordinates<T>::neighbor(int direction) const {
    return *this + directions.at(direction);
}

template <typename T>
std::array<HexCoordinates<T>, 6> HexCoordinates<T>::neighbors() const {
    std::array<HexCoordinates<T>, 6> res;
    std::generate(res.begin(), res.end(),
                  [i = 0, this]() mutable { return neighbor(i++); });
    return res;
}

template <typename T, typename U>
HexCoordinates<typename std::common_type<T, U>::type> operator*(
    const T &lhs, const HexCoordinates<U> &rhs) {
    return HexCoordinates<typename std::common_type<T, U>::type>(
        lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
}

template <typename T, typename U>
HexCoordinates<typename std::common_type<T, U>::type> operator*(
    const HexCoordinates<T> &lhs, const U &rhs) {
    return rhs * lhs;
}

struct Orientation {
   public:
    Orientation() = delete;
    // orientation matrix (used in the conversion to pixel point), row major
    std::array<float, 4> m;
    std::array<float, 4> minv;
    float start_angle;

   public:
    static const Orientation Pointy;
    static const Orientation Flat;
};

struct Layout {
    Orientation orientation;
    sf::Vector2f size;
    sf::Vector2f origin;

    sf::Vector2f cornerr_offset(int cornerr) const;
};

sf::Vector2f hex_to_world_point(const HexCoordinate &hex, const Layout &layout);
HexCoordinate hex_to_world_point(sf::Vector2f point, const Layout &layout);

#endif