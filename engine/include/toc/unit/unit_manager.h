#ifndef UNIT_MANAGER_H
#define UNIT_MANAGER_H

#include <map>
#include <typeindex>
#include <vector>

#include <cereal/types/vector.hpp>

#include "toc/core/id_gen.h"
#include "unit.h"
#include "unit_components.h"

struct ComponentVecBase {
    virtual ~ComponentVecBase() = default;
};

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

    using VecType = ComponentVec<Component>;

    auto& base_ptr = components[typeid(Component)];
    if (base_ptr == nullptr) {
        base_ptr = std::make_unique<VecType>();
    }
    auto ptr = static_cast<VecType*>(base_ptr.get());
    return ptr->vec;
}

class UnitManager {
   public:
    Unit::IdType create(UnitType type, const std::string& name,
                        bool assign_components = false);

    template <class Component, class... Args>
    Component& assign_component(Unit::IdType id, Args&&... args);

    template <class Component>
    Component* get_component(Unit::IdType id);

    template <class Component>
    void remove_component(Unit::IdType id);

    const std::map<Unit::IdType, Unit>& units() const noexcept;

    template <class Component>
    void apply_for_each(const std::function<bool(Component&)>& operation);

    static UnitManager create_test_manager();

    template <class Archive>
    void serialize(Archive& archive);

   private:
    void assign_default_components(Unit::IdType id, UnitType type);

    ComponentPoll _components{};

    std::map<Unit::IdType, Unit> _units{};
    IdGenerator<Unit::IdType> _id_gen{0};
};

template <class Archive>
void UnitManager::serialize(Archive& archive) {
    archive(
        CEREAL_NVP(_id_gen), CEREAL_NVP(_units),
        // list all possible components
        cereal::make_nvp("MovementComponents",
                         _components.get_container<MovementComponent>()),
        cereal::make_nvp("FightComponents", _components.get_container<FightComponent>()));
}

template <class Component, class... Args>
Component& UnitManager::assign_component(Unit::IdType id, Args&&... args) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);

    engine_assert_throw(_units.count(id) == 1, "Unit with id: {} does not exists.", id);

    auto& vec = _components.get_container<Component>();
    engine_assert_throw(std::none_of(vec.cbegin(), vec.cend(),
                                     [&id](const auto& cmp) { return cmp._owner == id; }),
                        "Reassigning a component: {} to a unit: {}",
                        typeid(Component).name(), id);

    auto& com       = vec.emplace_back(std::forward<Args>(args)...);
    com._owner      = id;
    com._owner_type = _units.at(id).type();
    return com;
}

template <class Component>
Component* UnitManager::get_component(Unit::IdType id) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);

    engine_assert_throw(_units.count(id) == 1, "Unit with id: {} does not exists.", id);

    auto& vec = _components.get_container<Component>();
    auto res  = std::find_if(vec.begin(), vec.end(),
                            [&id](auto& com) { return com._owner == id; });

    if (res != vec.end()) {
        return std::addressof(*res);
    } else {
        return nullptr;
    }
}

template <class Component>
void UnitManager::remove_component(Unit::IdType id) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);

    engine_assert_throw(_units.count(id) == 1, "Unit with id: {} does not exists.", id);

    auto& vec = _components.get_container<Component>();
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [&id](const auto& com) { return com._owner == id; }),
              vec.end());
}

template <class Component>
void UnitManager::apply_for_each(const std::function<bool(Component&)>& operation) {
    static_assert(std::is_base_of_v<ComponentBase, Component>);

    for (auto& cmp : _components.get_container<Component>())
        if (!operation(cmp))
            break;
}

#endif