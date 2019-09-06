#ifndef UNIT_H
#define UNIT_H

#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "core/log.h"

enum class UnitType { heavy, mechanized };

class Unit {
   public:
    using IdType = int;

    Unit(const Unit&) = delete;
    Unit& operator=(const Unit&) = delete;

   private:
    Unit() = default;

    void create_from_type(UnitType type);
    std::string name{};
    UnitType type{};
};

struct ComponentVecBase {};

template <class Component>
struct ComponentVec : public ComponentVecBase {
    std::vector<Component> vec;
};

class ComponentPoll {
   public:
    template <class Component>
    std::vector<Component>& get_container();

   private:
    std::map<std::type_info, std::unique_ptr<ComponentVecBase>> _components{};
};

template <class Component>
std::vector<Component>& ComponentPoll::get_container() {
    auto& base_ptr = _components[typeid(Component)];
    if (base_ptr == nullptr) {
        base_ptr = std::make_unique<ComponentVec<Component>>();
    }
    ComponentVec<Component>* ptr = base_ptr.get();
    return ptr->vec;
}

struct ComponentBase {
   private:
    friend class UnitManager;
    Unit::IdType owner{};
};

class UnitManager {
   public:
    using IdType = int;

    IdType create(UnitType type, std::string name);

    template <class Component, class... Args>
    Component& assign_component(IdType id, Args&&... args);

   private:
    ComponentPoll _components{};

    IdType _current_free_id{0};
    std::map<IdType, Unit> _units{};
};

template <class Component, class... Args>
Component& UnitManager::assign_component(IdType id, Args&&... args) {
    app_assert(_units.count(id) == 1, "Unit with id: {} does not exists.", id);
    auto& vec = _components.get_container<Component>();
    auto& com = vec.emplace_back(std::forward<Args>(args)...);
    com.owner = id;
    return com;
}

#endif