
#ifndef UNIT_H
#define UNIT_H

#include <string>

#include <cereal/types/string.hpp>

class Unit {
    friend class UnitManager;
   public:
    using IdType = int;
    using KindType = int;

    Unit() = default;

    const std::string& name() const noexcept;
    KindType type() const noexcept;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    std::string _name{};
    KindType _type{};
};

template <class Archive>
void Unit::serialize(Archive& archive) {
    archive(CEREAL_NVP(_name), CEREAL_NVP(_type));
}

struct ComponentBase {
    friend class UnitManager;
   public:
    constexpr Unit::IdType owner() const noexcept { return _owner; };
    constexpr Unit::KindType owner_type() const noexcept { return _owner_type; };

    template <class Archive>
    void serialize(Archive& archive);

   protected:
    constexpr ComponentBase() = default;

   private:
    Unit::IdType _owner{};
    Unit::KindType _owner_type{};
};

template <class Archive>
void ComponentBase::serialize(Archive& archive) {
    archive(CEREAL_NVP(_owner), CEREAL_NVP(_owner_type));
}

#endif