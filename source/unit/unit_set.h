#pragma once

#include <map>
#include <memory>
#include <type_traits>

#include<cereal/types/map.hpp>

class Unit;

class Unit_set {
   public:
    template <class Unit_type>
    void push_unit(const std::string& description);
    Unit* get_by_id(const int& id);
    bool remove_by_id(const int& id);

    template <class Archive>
    void serialize(Archive& ar);

   private:
    std::map<int, std::unique_ptr<Unit>> _units{};
};

template <class Unit_type>
void Unit_set::push_unit(const std::string& description) {
    static_assert(std::is_base_of<Unit, Unit_type>::value, "Cannot add to Unit_set a non Unit type.");
    auto u = std::make_shared<Unit_type>(description);
    _units.emplace({u->get_id(), u});
}

template <class Archive>
void Unit_set::serialize(Archive& ar) {
    ar(_units);
}
