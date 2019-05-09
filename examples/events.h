#pragma once

#include <list>
#include <memory>
#include <map>
#include <vector>
#include <functional>

class IEventData {
public:
    typedef size_t id_t; 
    virtual id_t GetID() = 0; 
}; 

typedef std::shared_ptr<IEventData> IEventDataPtr; 
typedef std::function<void(IEventDataPtr&)> EventDelegate; 

class IEventManager {
public:
    virtual bool AddListener(IEventData::id_t id, EventDelegate proc) = 0;
    virtual bool RemoveListener(IEventData::id_t id, EventDelegate proc) = 0; 
    virtual void QueueEvent(IEventDataPtr ev) = 0; 
    virtual void ProcessEvents() = 0; 
}; 


#define DECLARE_EVENT(type) \
    static IEventData::id_t ID(){ \
        return reinterpret_cast<IEventData::id_t>(&ID); \
    } \
    IEventData::id_t GetID() override { \
        return ID(); \
    }\

class EventManager : public IEventManager {
public:
    typedef std::list<EventDelegate> EventDelegateList; 

    ~EventManager(){
    } 
    //! Adds a listener to the event. The listener should invalidate itself when it needs to be removed. 
    virtual bool AddListener(IEventData::id_t id, EventDelegate proc) override; 

    //! Removes the specified delegate from the list
    virtual bool RemoveListener(IEventData::id_t id, EventDelegate proc) override; 

    //! Queues an event to be processed during the next update
    virtual void QueueEvent(IEventDataPtr ev) override; 

    //! Processes all events
    virtual void ProcessEvents() override; 
private:
    std::list<std::shared_ptr<IEventData>> mEventQueue; 
    std::map<IEventData::id_t, EventDelegateList> mEventListeners; 

}; 

//! Helper class that automatically handles removal of individual event listeners registered using OnEvent() member function upon destruction of an object derived from this class. 
class EventListener {
public:
    //! Template function that also converts the event into the right data type before calling the event listener. 
    template<class T>
    bool OnEvent(std::function<void(std::shared_ptr<T>)> proc){
        return OnEvent(T::ID(), [&, proc](IEventDataPtr data){
            auto ev = std::dynamic_pointer_cast<T>(data); 
            if(ev) proc(ev); 
        }); 
    }
protected:
    typedef std::pair<IEventData::id_t, EventDelegate> _EvPair; 
    EventListener(std::weak_ptr<IEventManager> mgr):_els_mEventManager(mgr){

    }
    virtual ~EventListener(){
        if(_els_mEventManager.expired()) return; 
        auto em = _els_mEventManager.lock(); 
        for(auto i : _els_mLocalEvents){
            em->RemoveListener(i.first, i.second); 
        }
    }

    bool OnEvent(IEventData::id_t id, EventDelegate proc){
        if(_els_mEventManager.expired()) return false; 
        auto em = _els_mEventManager.lock(); 
        if(em->AddListener(id, proc)){
            _els_mLocalEvents.push_back(_EvPair(id, proc)); 
        }
        return true;
    }
private:
    std::weak_ptr<IEventManager> _els_mEventManager; 
    std::vector<_EvPair>        _els_mLocalEvents; 
    //std::vector<_DynEvPair> mDynamicLocalEvents; 
}; 