#pragma once

#include <SFML/Network.hpp>

class Server {
   public:
    void listen_at_port(const unsigned short& port);
    void accept_client();

    sf::IpAddress get_local_ip() const;
    sf::IpAddress get_public_ip() const;
    unsigned short get_port() const;


   private:
    sf::TcpSocket _socket;
    sf::TcpListener _listener;
};
