#include "../source/messaging/messaging.h"

#include <iostream>

const Message::id_type Message::name = "Message";

class Hello_msg : public Message {
   public:
    void print() const;
    static const Message::id_type name;

    virtual Message::id_type get_name() const override { return name; }

    static Message::ptr_base create(const std::string& str) { return std::make_shared<Hello_msg>(); }

   private:
    std::string _msg = "hello";
};

Message::Registrar<Hello_msg> registrar();

const Message::id_type Hello_msg::name = "Hello_msg";

void Hello_msg::print() const {
    std::cout << _msg << "\n";
}

class Listener : public Message_listener {
   public:
    Listener(std::weak_ptr<Message_bus> mb);
};

Listener::Listener(std::weak_ptr<Message_bus> mb) : Message_listener(mb) {
    register_handler<Hello_msg>([](std::shared_ptr<Hello_msg>& msg) { msg->print(); });
};

int main() {
    auto bus = std::make_shared<Message_bus>();
    Listener list(bus);
    bus->queue_message(std::make_shared<Hello_msg>());
    bus->distribute_messages();
}