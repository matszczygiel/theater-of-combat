#ifndef UNIT_H
#define UNIT_H

#include <string>

#include <cereal/types/string.hpp>

enum class UnitType { heavy, mechanized };

class Unit {
   public:
    using IdType = int;

    const std::string& name() const noexcept;
    UnitType type() const noexcept;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    friend class UnitManager;

    std::string _name{};
    UnitType _type{};
};

template <class Archive>
void Unit::serialize(Archive& archive) {
    archive(CEREAL_NVP(_name), CEREAL_NVP(_type));
}

struct ComponentBase {
    Unit::IdType owner() const noexcept;
    UnitType owner_type() const noexcept;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    friend class UnitManager;

    Unit::IdType _owner{};
    UnitType _owner_type{};
};

template <class Archive>
void ComponentBase::serialize(Archive& archive) {
    archive(CEREAL_NVP(_owner), CEREAL_NVP(_owner_type));
}

#endif