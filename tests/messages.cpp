#include "../source/messaging/messaging.h"

#include <iostream>

class Hello_msg : public Message {
   public:
    void print() const;
    Message::id_type name() const { return "Hello_msg"; }

   private:
    std::string _msg = "hello";
};

void Hello_msg::print() const {
    std::cout << _msg << "\n";
}

class Listener : public Message_listener {
   public:
    Listener(std::weak_ptr<Message_bus> mb);
    virtual ~Listener();
};

Listener::Listener(std::weak_ptr<Message_bus> mb) : Message_listener(mb) {
    respond<Hello_msg>([](std::unique_ptr<Hello_msg>& msg) { msg->print(); });
};

Listener::~Listener() {
    ~Message_listener();
};

int main() {
    std::shared_ptr<Message_bus> bus;
    Listener list(bus);
    Hello_msg msg;
    bus->distribute_messages();

}