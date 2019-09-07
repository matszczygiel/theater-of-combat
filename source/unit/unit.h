#ifndef UNIT_H
#define UNIT_H

#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <vector>

#include "core/log.h"

enum class UnitType { heavy, mechanized };

class Unit {
   public:
    using IdType = int;

    Unit()            = default;
    Unit(const Unit&) = delete;
    Unit& operator=(const Unit&) = delete;

   private:
    friend class UnitManager;

    std::string name{};
    UnitType type{};
};

struct ComponentBase {
   private:
    friend class UnitManager;
    Unit::IdType owner{};
};

struct ComponentVecBase {};

template <class Component>
struct ComponentVec : public ComponentVecBase {
    static_assert(std::is_base_of_v<ComponentBase, Component>);
    std::vector<Component> vec;
};

struct ComponentPoll {
    template <class Component>
    std::vector<Component>& get_container();

    std::map<std::type_index, std::unique_ptr<ComponentVecBase>> components{};
};

template <class Component>
std::vector<Component>& ComponentPoll::get_container() {
    static_assert(std::is_base_of_v<ComponentBase, Component>);
    auto& base_ptr = components[typeid(Component)];
    if (base_ptr == nullptr) {
        base_ptr = std::make_unique<ComponentVec<Component>>();
    }
    auto ptr = static_cast<ComponentVec<Component>*>(base_ptr.get());
    return ptr->vec;
}

class UnitManager {
   public:
    Unit::IdType create(UnitType type, std::string name,
                        bool assign_components = false);

    template <class Component, class... Args>
    Component& assign_component(Unit::IdType id, Args&&... args);

    template <class Component>
    void remove_component(Unit::IdType id);

   private:
    void assign_default_components(Unit::IdType id, UnitType type);

    ComponentPoll _components{};

    Unit::IdType _current_free_id{0};
    std::map<Unit::IdType, Unit> _units{};
};

template <class Component, class... Args>
Component& UnitManager::assign_component(Unit::IdType id, Args&&... args) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);
    app_assert(_units.count(id) == 1, "Unit with id: {} does not exists.", id);

    auto& vec = _components.get_container<Component>();
    auto& com = vec.emplace_back(std::forward<Args>(args)...);
    com.owner = id;
    return com;
}

template <class Component>
void UnitManager::remove_component(Unit::IdType id) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);
    app_assert(_units.count(id) == 1, "Unit with id: {} does not exists.", id);

    auto& vec = _components.get_container<Component>();
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [&](auto& com) { return com.owner == id; }),
              vec.end());
}

#endif