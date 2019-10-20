#ifndef CONNECTION_H
#define CONNECTION_H

#include <type_traits>

#include <SFML/Network.hpp>

class Messenger {
   public:
    sf::IpAddress get_remote_ip() const;
    unsigned short get_remote_port() const;

    bool send(sf::Packet& packet);
    bool receive(sf::Packet& packet);

    void disconnect();
    
   protected:
    sf::TcpSocket _socket{};
};

class Server : public Messenger {
   public:
    bool listen_at_port(const unsigned short& port);
    bool accept_client();

    static sf::IpAddress get_local_ip();

    unsigned short get_port();

   private:
    sf::TcpListener _listener;
};

class Client : public Messenger {
   public:
    bool connect_to_server(const sf::IpAddress& ip, const unsigned short& port);
};

#endif