#pragma once

#include <SFML/Network.hpp>


class Client {
   public:
    bool connect_to_server(const sf::IpAddress& ip, const unsigned short& port);
    void disconnect();
    sf::IpAddress get_remote_ip() const;
    unsigned short get_remote_port() const;
    bool send(sf::Packet& packet);
    bool receive(sf::Packet& packet);

   private:
    sf::TcpSocket _socket;
};