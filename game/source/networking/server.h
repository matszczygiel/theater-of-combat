#pragma once

#include <SFML/Network.hpp>

class Server {
   public:
    bool listen_at_port(const unsigned short& port);
    bool accept_client();

    sf::IpAddress get_remote_ip() const;

    static sf::IpAddress get_local_ip();
    static sf::IpAddress get_public_ip();

    unsigned short get_port();

    bool send(sf::Packet& packet);
    bool receive(sf::Packet& packet);

   private:
    sf::TcpSocket _socket;
    sf::TcpListener _listener;
};