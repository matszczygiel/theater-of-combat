#ifndef CONNECTION_H
#define CONNECTION_H

#include <type_traits>

#include <SFML/Network.hpp>

class Messenger {
   public:
    sf::IpAddress get_remote_ip() const;
    unsigned short get_remote_port() const;
    void disconnect();

    bool send(sf::Packet& packet);
    bool receive(sf::Packet& packet);
    
   protected:
    sf::TcpSocket _socket{};
};

class Server : public Messenger {
   public:
    bool listen(const unsigned short& port);
    bool accept_client();
    unsigned short get_port();

    static sf::IpAddress get_local_ip();

   private:
    sf::TcpListener _listener;
};

class Client : public Messenger {
   public:
    bool connect(const sf::IpAddress& ip, const unsigned short& port);
};

#endif