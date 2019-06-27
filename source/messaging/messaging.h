#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <type_traits>

struct Message {
   public:
    template <class T>
    using ptr      = std::shared_ptr<T>;
    using ptr_base = ptr<Message>;
    using id_type  = std::size_t;

    template <class T>
    constexpr static id_type get_id() {
       // static_assert(std::is_base_of<Message, T>()::value, "Message id can only be given to message subclasses.");
        return typeid(T).hash_code();
    }

    virtual std::string log() const = 0;

    virtual ~Message() = default;
};

class Message_bus {
   public:
    using message_callback = std::function<void(Message::ptr_base&)>;

    virtual bool add_listener(const Message::id_type& id, message_callback callback);
    virtual bool remove_listener(const Message::id_type& id, message_callback callback);
    virtual void queue_message(Message::ptr_base message);
    virtual void distribute_messages();
    virtual void notify(Message::ptr_base message);

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
        return register_handler(Message::get_id<T>(), [&, callback](Message::ptr_base msg) {
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
