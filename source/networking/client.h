#pragma once

#include <SFML/Network.hpp>

class Client {
   public:
    void connect_to_server(const sf::IpAddress& ip, const unsigned short& port);
    void disconnect();

   private:
    sf::TcpSocket _socket;
};