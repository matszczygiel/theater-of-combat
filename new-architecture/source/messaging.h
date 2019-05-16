#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Message {
   public:
    template <class T>
    using ptr      = std::shared_ptr<T>;
    using ptr_base = ptr<Message>;

    using id_type = std::string;

    using create_method_type = std::function<ptr_base(const std::string&)>;

    static const id_type name;
    virtual id_type get_name() const { return name; }
    virtual ~Message() = default;

    static ptr_base instantiate(const id_type& name, const std::string& stream);
    static bool register_class(const id_type& class_name, create_method_type callback);

    template <class Derived>
    struct Registrar {
        Registrar() {
            Message::register_class(Derived::name, Derived::create);
        }
    };

   private:
    static std::unordered_map<id_type, create_method_type> _register;
};

class Message_bus {
   public:
    using message_callback = std::function<void(Message::ptr_base&)>;

    virtual bool add_listener(const Message::id_type& id, message_callback callback);
    virtual bool remove_listener(const Message::id_type& id, message_callback callback);
    virtual void queue_message(Message::ptr_base message);
    virtual void distribute_messages();

    virtual ~Message_bus() = default;

   private:
    std::list<Message::ptr_base> _queue;
    std::map<Message::id_type, std::list<message_callback>> _listeners;
};

class Message_listener {
   public:
    template <class T>
    bool register_handler(std::function<void(Message::ptr<T>&)> callback) {
        static_assert(std::is_base_of<Message, T>::value, "Trying to register handler with non-message class.");
        return register_handler(T::name, [&, callback](Message::ptr_base msg) {
            auto ev = std::dynamic_pointer_cast<T>(msg);
            if (ev)
                callback(ev);
        });
    }

   protected:
    Message_listener(std::weak_ptr<Message_bus> bus) : _message_bus(bus){};

    virtual ~Message_listener();

    bool register_handler(Message::id_type id, Message_bus::message_callback callback);

    using message_handler_pair = std::pair<Message::id_type, Message_bus::message_callback>;

   private:
    std::weak_ptr<Message_bus> _message_bus;
    std::vector<message_handler_pair> _registered_handlers;
};
