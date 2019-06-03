#include "messaging.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

bool Message_bus::add_listener(const Message::id_type &id, message_callback callback) {
    auto i = _listeners.find(id);
    if (i == _listeners.end()) {
        _listeners[id] = std::list<message_callback>();
    }
    auto &list = _listeners[id];
    for (auto i = list.begin(); i != list.end(); ++i) {
        auto &func = *i;
        if (func.target<message_callback>() == callback.target<message_callback>())
            return false;
    }
    list.push_back(callback);
    return true;
}

bool Message_bus::remove_listener(const Message::id_type &id, message_callback callback) {
    auto j = _listeners.find(id);
    if (j == _listeners.end())
        return false;
    auto &list = j->second;
    for (auto i = list.begin(); i != list.end(); ++i) {
        auto &func = *i;
        if (func.target<message_callback>() == callback.target<message_callback>()) {
            list.erase(i);
            return true;
        }
    }
    return false;
}

void Message_bus::queue_message(Message::ptr_base message) {
    _queue.push_back(message);
}

void Message_bus::notify(Message::ptr_base message) {
    auto listeners = _listeners.find(typeid(*message).hash_code());
    if (listeners != _listeners.end()) {
        for (auto l : listeners->second) {
            l(message);
        }
    }
}

void Message_bus::distribute_messages() {
    auto count = _queue.size();
    for (auto it = _queue.begin(); it != _queue.end(); ++it) {
        if (!count)
            break;
        notify(*it);

        it = _queue.erase(it);
        count--;
    }
}

Message_listener::~Message_listener() {
    if (_message_bus.expired())
        return;
    auto em = _message_bus.lock();
    for (auto i : _registered_handlers) {
        em->remove_listener(i.first, i.second);
    }
}

bool Message_listener::register_handler(Message::id_type id, Message_bus::message_callback callback) {
    if (_message_bus.expired())
        return false;
    auto em = _message_bus.lock();
    if (em->add_listener(id, callback)) {
        _registered_handlers.push_back(message_handler_pair(id, callback));
        return true;
    }
    return false;
}

