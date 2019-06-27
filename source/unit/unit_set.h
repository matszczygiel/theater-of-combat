#pragma once

#include <functional>
#include <map>
#include <memory>
#include <type_traits>
#include <set>

#include <cereal/types/map.hpp>

#include "unit.h"

class Unit_set {
   public:
    template <class Unit_type>
    int push_unit(const std::string& description);

    Unit* get_by_id(const int& id);
    bool remove_by_id(const int& id);
    void init_tokens(const float& size);

    void apply(const std::function<void(Unit&)>& function);
    void apply(const std::vector<int> ids, const std::function<void(Unit&)>& function);
    void apply(const std::set<int> ids, const std::function<void(Unit&)>& function);

    template <class Archive>
    void serialize(Archive& ar);

   private:
    std::map<int, std::unique_ptr<Unit>> _units{};
};

template <class Unit_type>
int Unit_set::push_unit(const std::string& description) {
    static_assert(std::is_base_of<Unit, Unit_type>::value, "Cannot add to Unit_set a non Unit type.");
    std::unique_ptr<Unit> u = std::make_unique<Unit_type>(description);
    auto id                 = u->get_id();
    _units.emplace(std::make_pair(id, std::move(u)));
    return id;
}

template <class Archive>
void Unit_set::serialize(Archive& ar) {
    ar(_units);
}
