#pragma once

#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Message {
   public:
    using ptr                = std::shared_ptr<Message>;
    using create_method_type = std::function<std::unique_ptr<Message>(const std::string&)>;


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

typedef std::function<void(IEventDataPtr&)> EventDelegate;

class Message_bus {
   public:
    virtual bool add_listener(IEventData::id_t id, EventDelegate proc)    = 0;
    virtual bool remove_listener(IEventData::id_t id, EventDelegate proc) = 0;
    virtual void queue_event(Message::ptr ev)                             = 0;
    virtual void process_events()                                         = 0;
};

#define DECLARE_EVENT(type)                             \
    static IEventData::id_t ID() {                      \
        return reinterpret_cast<IEventData::id_t>(&ID); \
    }                                                   \
    IEventData::id_t GetID() override {                 \
        return ID();                                    \
    }

class EventManager : public IEventManager {
   public:
    typedef std::list<EventDelegate> EventDelegateList;

    ~EventManager() {
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
    template <class T>
    bool OnEvent(std::function<void(std::shared_ptr<T>)> proc) {
        return OnEvent(T::ID(), [&, proc](IEventDataPtr data) {
            auto ev = std::dynamic_pointer_cast<T>(data);
            if (ev)
                proc(ev);
        });
    }

   protected:
    typedef std::pair<IEventData::id_t, EventDelegate> _EvPair;
    EventListener(std::weak_ptr<IEventManager> mgr) : _els_mEventManager(mgr) {
    }
    virtual ~EventListener() {
        if (_els_mEventManager.expired())
            return;
        auto em = _els_mEventManager.lock();
        for (auto i : _els_mLocalEvents) {
            em->RemoveListener(i.first, i.second);
        }
    }

    bool OnEvent(IEventData::id_t id, EventDelegate proc) {
        if (_els_mEventManager.expired())
            return false;
        auto em = _els_mEventManager.lock();
        if (em->AddListener(id, proc)) {
            _els_mLocalEvents.push_back(_EvPair(id, proc));
        }
    }

   private:
    std::weak_ptr<IEventManager> _els_mEventManager;
    std::vector<_EvPair> _els_mLocalEvents;
    //std::vector<_DynEvPair> mDynamicLocalEvents;
};
