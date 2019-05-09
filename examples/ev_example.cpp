#include <functional>
#include <memory>
#include <stdio.h>
#include <list>
#include <map>
#include <iostream>

#include "events.h"

using namespace std; 

class DisplayTextEvent : public IEventData {
public:
    DECLARE_EVENT(DisplayTextEvent); 

    DisplayTextEvent(const string &text){
        mStr = text; 
    }
    ~DisplayTextEvent(){
        printf("Deleted event data\n"); 
    }
    const string &GetText(){
        return mStr; 
    }
private:
    string mStr; 
}; 

class Emitter { 
public:
    Emitter(shared_ptr<IEventManager> em){
        mEmgr = em; 
    }
    void EmitEvent(){
        mEmgr->QueueEvent(shared_ptr<IEventData>(
            new DisplayTextEvent("Hello World!"))); 
    }
private:
    shared_ptr<IEventManager> mEmgr; 
}; 

class Receiver : public EventListener{
public:
    Receiver(shared_ptr<IEventManager> em) : EventListener(em){
        mEmgr = em; 

        OnEvent<DisplayTextEvent>([&](shared_ptr<DisplayTextEvent> data){
            printf("It's working: %s\n", data->GetText().c_str()); 
        }); 
    }
    ~Receiver(){
        bool res = mEmgr->RemoveListener(DisplayTextEvent::ID(), std::bind(&Receiver::OnExampleEvent, this, placeholders::_1)); 
        std::cout << res <<'\n';
    }
    void OnExampleEvent(IEventDataPtr &data){
        auto ev = dynamic_pointer_cast<DisplayTextEvent>(data); 
        if(!ev) return; 
        printf("Received event: %s\n", ev->GetText().c_str()); 
    }
private:
    shared_ptr<IEventManager> mEmgr; 
}; 

int main(){
    auto emgr = shared_ptr<IEventManager>(new EventManager()); 


    Emitter emit(emgr); 
    {
        Receiver receive(emgr); 

        emit.EmitEvent(); 
        emgr->ProcessEvents(); 
    }
    emit.EmitEvent(); 
    emgr->ProcessEvents(); 
    emgr = 0; 

    return 0; 
}