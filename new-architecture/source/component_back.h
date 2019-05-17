#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

template <class Base, class... Constructor_parameters>
class Component {
   private:
    template <class T>
    static std::shared_ptr<T> create_component(Constructor_parameters... params) {
        static_assert(std::is_base_of<Base, T>::value, "Trying to register non component class.");
        return std::make_shared<T>(params);
    }

    using register_type = std::map<std::string, std::function<std::shared_ptr<Base>(Constructor_parameters...)>>;

    static register_type& get_register() {
        static register_type reg;
        return reg;
    }

   public:
    template <class T>
    struct Register_entry {
        Register_entry(const std::string& name) {
            auto& reg = get_register();
            reg[name] = create_component<T>;
        }
    };

    static std::shared_ptr<Base> create(const std::string& name, Constructor_parameters... params) {
        auto& reg = get_register();
        auto it   = reg.find(name);

        if (it == reg.end()) {
            return nullptr;
        }

        return it->second();
    }
};

#define COMPONENT_REGISTER(TYPE, NAME) \
    static const Component::Register_entry<TYPE> reg_ent_##TYPE(NAME);