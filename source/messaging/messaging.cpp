#include "messaging.h"



bool Message_bus::add_listener(const Message::id_type& id, message_callback callback){
    auto i = _listeners.find(id); 
    if(i == _listeners.end()){
        _listeners[id] = std::list<message_callback>(); 
    }
    auto &list = _listeners[id]; 
    for(auto i = list.begin(); i != list.end(); ++i){
        auto &func = *i; 
        if(func.target<message_callback>() == callback.target<message_callback>()) 
            return false; 
    }
    list.push_back(callback); 
}

bool Message_bus::remove_listener(const Message::id_type& id, message_callback callback){
    auto j = _listeners.find(id); 
    if(j == _listeners.end()) return false; 
    auto &list = j->second; 
    for(auto i = list.begin(); i != list.end(); ++i){
        auto &func = *i; 
        if(func.target<message_callback>() == callback.target<message_callback>()) {
            list.erase(i); 
            return true; 
        }
    }
    return false; 
}

void Message_bus::queue_message(Message::ptr message) {
    _queue.push_back(message); 
}

void Message_bus::distribute_messages(){
    auto count = _queue.size(); 
    for(auto it = _queue.begin(); it != _queue.end(); ++it){
        if(!count) break; 
        auto &i = *it; 
        auto listeners = _listeners.find(i->name()); 
        if(listeners != _listeners.end()){
            for(auto l : listeners->second){
                l(i); 
            }
        }

        it = _queue.erase(it); 
        count--; 
    }
}