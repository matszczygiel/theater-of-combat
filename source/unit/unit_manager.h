#ifndef UNIT_MANAGER_H
#define UNIT_MANAGER_H

#include <map>
#include <typeindex>
#include <vector>

#include <cereal/types/vector.hpp>

#include "unit.h"
#include "unit_components.h"

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
    Unit::IdType create(UnitType type, std::string name, bool assign_components = false);

    template <class Component, class... Args>
    Component& assign_component(Unit::IdType id, Args&&... args);

    template <class Component>
    Component* get_component(Unit::IdType id);

    template <class Component>
    void remove_component(Unit::IdType id);

    const std::map<Unit::IdType, Unit>& units() const;

    template <class Component>
    void apply_for_each(const std::function<bool(Component&)>& operation);

    static UnitManager create_test_manager();

    template <class Archive>
    void serialize(Archive& archive);

   private:
    void assign_default_components(Unit::IdType id, UnitType type);

    ComponentPoll _components{};

    Unit::IdType _current_free_id{0};
    std::map<Unit::IdType, Unit> _units{};
};

template <class Archive>
void UnitManager::serialize(Archive& archive) {
    archive(
        CEREAL_NVP(_current_free_id), CEREAL_NVP(_units),
        // list all posible components
        cereal::make_nvp("MovementComponents",
                         _components.get_container<MovementComponent>()),
        cereal::make_nvp("FightComponents", _components.get_container<FightComponent>()));
}

template <class Component, class... Args>
Component& UnitManager::assign_component(Unit::IdType id, Args&&... args) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);
    app_assert(_units.count(id) == 1, "Unit with id: {} does not exists.", id);

    auto& vec = _components.get_container<Component>();
    app_assert(
        std::none_of(vec.begin(), vec.end(), [&](auto& cmp) { return cmp._owner == id; }),
        "Reassigning a component: {} to a unit: {}", typeid(Component).name(), id);

    auto& com       = vec.emplace_back(std::forward<Args>(args)...);
    com._owner      = id;
    com._owner_type = _units[id].type();
    return com;
}

template <class Component>
Component* UnitManager::get_component(Unit::IdType id) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);
    app_assert(_units.count(id) == 1, "Unit with id: {} does not exists.", id);

    auto& vec = _components.get_container<Component>();

    auto res =
        std::find_if(vec.begin(), vec.end(), [&](auto& com) { return com._owner == id; });

    if (res != vec.end()) {
        return &(*res);
    } else {
        return nullptr;
    }
}

template <class Component>
void UnitManager::remove_component(Unit::IdType id) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);
    app_assert(_units.count(id) == 1, "Unit with id: {} does not exists.", id);

    auto& vec = _components.get_container<Component>();
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [&](auto& com) { return com._owner == id; }),
              vec.end());
}

template <class Component>
void UnitManager::apply_for_each(const std::function<bool(Component&)>& operation) {
    for (auto& cmp : _components.get_container<Component>())
        if (!operation(cmp))
            break;
}

#endif