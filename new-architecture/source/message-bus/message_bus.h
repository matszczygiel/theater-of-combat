#pragma once

#include <list>
#include <map>

namespace toc {

class Message {};

class Message_bus {
   public:
    typedef std::list<EventDelegate> EventDelegateList;

    virtual bool AddListener(IEventData::id_t id, EventDelegate proc) override;

    virtual bool RemoveListener(IEventData::id_t id, EventDelegate proc) override;
    virtual void QueueEvent(IEventDataPtr ev) override;
    virtual void ProcessEvents() override;

   private:
    std::list<std::shared_ptr<IEventData>> mEventQueue;
    std::map<IEventData::id_t, EventDelegateList> mEventListeners;
};

class Message_reciver {
};

class Message_emmiter {
};
}