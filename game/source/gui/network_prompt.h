#ifndef NETWORK_PROMPT_H
#define NETWORK_PROMPT_H

#include<variant>

#include "networking/connection.h"

void show_network_prompt(std::variant<Server, Client>& net, std::string title, bool* open);


#endif