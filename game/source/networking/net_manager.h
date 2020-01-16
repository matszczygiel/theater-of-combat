#ifndef NET_MANAGER_H
#define NET_MANAGER_H

#include <variant>

#include "connection.h"

class NetManager {
   public:
    bool setup_server();
    bool setup_client(const sf::IpAddress& ip);

    void update();
    void close_connection();

    bool is_server() const;
    bool is_client() const;
    bool is_done() const;

    constexpr static unsigned short port = 28008;

    std::variant<Server, Client> net{};

   private:
    bool _need_update{false};
};

#endif