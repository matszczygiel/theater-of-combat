#pragma once

#include <SFML/Network.hpp>

class Client {
   public:
    Client() = delete;

    static void connect_to_server(const sf::IpAddress& ip, const unsigned short& port);
    static void disconnect();

   private:
    static sf::TcpSocket _socket;
};