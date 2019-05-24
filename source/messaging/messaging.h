#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "core/registrable.h"

class Message : public Registrable<Message, std::string, std::string> {
   public:
    template <class T>
    using ptr              = std::shared_ptr<T>;
    using ptr_base         = ptr<Message>;
    using id_type          = std::string;
    using registrable_base = Registrable<Message, std::string, std::string>;

    virtual inline id_type get_name() const = 0;
    virtual std::string to_string() const   = 0;

    virtual ~Message() = default;

    static ptr_base create(const std::string& streamline);

   protected:
    template <class T>
    static T get_entry_from_stream(const std::string& stream, std::string token) {
        token += "\\s+\\S+";
        std::regex rg(token, std::regex_constants::icase);
        std::smatch match;
        T res{};
        if (std::regex_search(stream, match, rg)) {
            std::stringstream ss(match.str());
            ss.get();
            ss >> res;
        }
        return res;
    }
};

#define DEFINE_MESSAGE_NAMING(Message_class)                                        \
    static constexpr auto name = #Message_class;                                    \
    virtual inline id_type get_name() const override { return #Message_class; };

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
