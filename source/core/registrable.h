#pragma once

#include <functional>
#include <map>
#include <memory>
#include <type_traits>

template <class Base, class Id, class... Constructor_parameters>
class Registrable {
   public:
    template <class T>
    struct Register_entry {
        Register_entry(const Id& name);
    };

    static std::shared_ptr<Base> create(const Id& name, Constructor_parameters... params);

   private:
    template <class T>
    static std::shared_ptr<T> create_registrable(Constructor_parameters&&... params);

    using register_type = std::map<Id, std::function<std::shared_ptr<Base>(Constructor_parameters...)>>;

    static register_type& get_register();
};

#define REGISTER_CLASS(REGISTRABLE_BASE, TYPE, ID)                                 \
    namespace {                                                                    \
    static const REGISTRABLE_BASE::Register_entry<TYPE> register_entry_##TYPE(ID); \
    }

template <class Base, class Id, class... Constructor_parameters>
template <class T>
std::shared_ptr<T> Registrable<Base, Id, Constructor_parameters...>::create_registrable(Constructor_parameters&&... params) {
    static_assert(std::is_base_of<Base, T>::value, "Trying to register non registrable class.");
    return std::make_shared<T>(std::forward<Constructor_parameters>(params)...);
}

template <class Base, class Id, class... Constructor_parameters>
Registrable<Base, Id, Constructor_parameters...>::register_type&
Registrable<Base, Id, Constructor_parameters...>::get_register() {
    static register_type reg;
    return reg;
}

template <class Base, class Id, class... Constructor_parameters>
std::shared_ptr<Base> Registrable<Base, Id, Constructor_parameters...>::create(const Id& name, Constructor_parameters... params) {
    auto& reg = get_register();
    auto it   = reg.find(name);

    if (it == reg.end()) {
        return nullptr;
    }

    return it->second(std::forward<Constructor_parameters>(params)...);
}

template <class Base, class Id, class... Constructor_parameters>
template <class T>
Registrable<Base, Id, Constructor_parameters...>::Register_entry<T>::Register_entry(const Id& name) {
    auto& reg = Registrable<Base, Id, Constructor_parameters...>::get_register();
    reg[name] = Registrable<Base, Id, Constructor_parameters...>::create_registrable<T>;
}