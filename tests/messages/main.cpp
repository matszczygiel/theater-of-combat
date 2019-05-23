#include <iostream>
#include <string>

#include "messaging/concrete_message.h"

int main() {
    std::string line = "Request_unit_movement h 23 u uu n nn";
    auto msg = Message::create(line);
    std::cout << msg->to_string();
}