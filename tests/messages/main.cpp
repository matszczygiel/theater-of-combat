#include <iostream>
#include <string>

#include "messaging/concrete_message.h"

int main() {
    std::string line;
    std::getline(std::cin, line);
    auto msg = Message::registrable_base::create("Request_unit_movement", line);
    std::cout << msg->to_string();
}