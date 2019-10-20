#ifndef NET_MANAGER_H
#define NET_MANAGER_H

#include <variant>

#include "connection.h"

class NetManager {
   public:
    bool setup_server(sf::IpAddress& ip, int& port);
    bool setup_client(const sf::IpAddress& ip, const int& port);

    void update();
    void close_connection();

    bool is_server() const;
    bool is_client() const;
    bool id_done() const;

    std::variant<Server, Client> net;

   private:
    bool _need_update{false};
};

#endif