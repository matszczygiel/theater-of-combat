#pragma once

#include <SFML/Network.hpp>

#include "netwoking_status.h"

class Client {
   public:
    bool connect_to_server(const sf::IpAddress& ip, const unsigned short& port);
    void disconnect();

   private:
    sf::TcpSocket _socket;
};