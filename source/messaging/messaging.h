#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Message {
   public:
    using ptr                = std::unique_ptr<Message>;
    using create_method_type = std::function<std::unique_ptr<Message>(const std::string&)>;
    using id_type            = std::string;

    virtual id_type name() const = 0;

    template <class Derived>
    struct Registrar {
        Registrar() {
            Message_factory::register_class(Derviced::name, Derived::create);
        }
    };

   private:
    static bool register_class(const std::string& class_name, create_method_type);

    static std::unordered_map<std::string, create_method_type> _register;
};

class Message_bus {
   public:
    using message_callback = std::function<void(Message::ptr&)>;

    virtual bool add_listener(const Message::id_type& id, message_callback callback);
    virtual bool remove_listener(const Message::id_type& id, message_callback callback);
    virtual void queue_message(Message::ptr message);
    virtual void distribute_messages();

   private:
    std::list<Message::ptr> _queue;
    std::map<Message::id_type, std::list<message_callback>> _listeners;
};

class Message_listener {
   public:
    template <class T>
    bool respond(std::function<void(std::unique_ptr<T>&)>) {
        return respond(T::name(), [&, callback](Message::ptr msg) {
            auto ev = std::dynamic_pointer_cast<T>(msg);
            if (ev)
                callback(ev);
        });
    }

   protected:
    Message_listener(std::weak_ptr<Message_bus> bus) : _message_bus(bus){};

    virtual ~Message_listener() {
        if (_message_bus.expired())
            return;
        auto em = _message_bus.lock();
        for (auto i : _registered_handlers) {
            em->remove_listener(i.first, i.second);
        }
    }

    bool respond(Message::id_type id, Message_bus::message_callback callback) {
        if (_message_bus.expired())
            return false;
        auto em = _message_bus.lock();
        if (em->add_listener(id, callback)) {
            _registered_handlers.push_back(message_handler_pair(id, callback));
        }
    }

    using message_handler_pair = std::pair<Message::id_type, Message_bus::message_callback>;

   private:
    std::weak_ptr<Message_bus> _message_bus;
    std::vector<message_handler_pair> _registered_handlers;
};
