#include <iostream>
#include <string>

#include "messaging/concrete_message.h"

int main() {
    std::string line = "h 23 u uu n nn";
    auto msg = Message::registrable_base::create("Request_unit_movement", line);
    std::cout << msg->to_string();
}