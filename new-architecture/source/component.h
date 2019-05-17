#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

class Component {
   public:
    static std::shared_ptr<Component> create(const std::string& name);

    virtual void do_work() = 0;
    virtual ~Component() = default;

   private:
    template <class T>
    static std::shared_ptr<T> create_component() {
        static_assert(std::is_base_of<Component, T>::value, "Trying to register non component class.");
        return std::make_shared<T>();
    }

    static std::map<std::string, std::function<std::shared_ptr<Component>(void)>>& get_register();

   public:
    template <class T>
    struct Register_entry {
        Register_entry(const std::string& name) {
            auto& reg = get_register();
            reg[name] = create_component<T>;
        }
    };
};

#define COMPONENT_REGISTER(TYPE, NAME)                                 \
    static const Component::Register_entry<TYPE> reg_ent_##TYPE(NAME); 