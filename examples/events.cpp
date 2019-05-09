#include "events.h"

using namespace std; 

bool EventManager::AddListener(IEventData::id_t id, EventDelegate proc){
    auto i = mEventListeners.find(id); 
    if(i == mEventListeners.end()){
        mEventListeners[id] = list<EventDelegate>(); 
    }
    auto &list = mEventListeners[id]; 
    for(auto i = list.begin(); i != list.end(); i++){
        EventDelegate &func = *i; 
        if(func.target<EventDelegate>() == proc.target<EventDelegate>()) 
            return false; 
    }
    list.push_back(proc); 
    return true;
}

bool EventManager::RemoveListener(IEventData::id_t id, EventDelegate proc){
    auto j = mEventListeners.find(id); 
    if(j == mEventListeners.end()) return false; 
    auto &list = j->second; 
    for(auto i = list.begin(); i != list.end(); ++i){
        EventDelegate &func = *i; 
        if(func.target<EventDelegate>() == proc.target<EventDelegate>()) {
            list.erase(i); 
            return true; 
        }
    }
    return false; 
}

void EventManager::QueueEvent(IEventDataPtr ev) {
    mEventQueue.push_back(ev); 
}

void EventManager::ProcessEvents(){
    size_t count = mEventQueue.size(); 
    for(auto it = mEventQueue.begin(); it != mEventQueue.end(); ++it){
        printf("Processing event..\n"); 
        if(!count) break; 
        auto &i = *it; 
        auto listeners = mEventListeners.find(i->GetID()); 
        if(listeners != mEventListeners.end()){
            // Call listeners
            for(auto l : listeners->second){
                l(i); 
            }
        }
        // remove event
        it = mEventQueue.erase(it); 
        count--; 
    }
}