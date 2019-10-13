
#ifndef UNIT_H
#define UNIT_H

#include <string>

#include <cereal/types/string.hpp>

enum class UnitType { heavy, mechanized };

class Unit {
    friend class UnitManager;

   public:
    using IdType = int;

    Unit() = default;

    const std::string& name() const noexcept;
    UnitType type() const noexcept;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    std::string _name{};
    UnitType _type{};
};

template <class Archive>
void Unit::serialize(Archive& archive) {
    archive(CEREAL_NVP(_name), CEREAL_NVP(_type));
}

struct ComponentBase {
    friend class UnitManager;
   public:
    constexpr Unit::IdType owner() const noexcept { return _owner; };
    constexpr UnitType owner_type() const noexcept { return _owner_type; };

    template <class Archive>
    void serialize(Archive& archive);

   protected:
    constexpr ComponentBase() = default;

   private:
    Unit::IdType _owner{};
    UnitType _owner_type{UnitType::mechanized};
};

template <class Archive>
void ComponentBase::serialize(Archive& archive) {
    archive(CEREAL_NVP(_owner), CEREAL_NVP(_owner_type));
}

#endif