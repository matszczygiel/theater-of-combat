#include "net_manager.h"

#include "toc/core/log.h"

bool NetManager::setup_server(sf::IpAddress& ip, int& port) {
    if (!is_server()) {
        net.emplace<Server>();
    }

    auto& server = std::get<Server>(net);
    _need_update = server.listen_at_port(sf::Socket::AnyPort);
    ip           = Server::get_local_ip();
    port         = server.get_port();
    return _need_update;
}

bool NetManager::setup_client(const sf::IpAddress& ip, const int& port) {
    engine_assert(port >= 0 && port < 65536, "Invalid port number");
    if (!is_client()) {
        net.emplace<Client>();
    }
    auto& client = std::get<Client>(net);
    _need_update = false;
    return client.connect_to_server(ip, port);
}

void NetManager::update() {
    if (!_need_update)
        return;

    if (is_server()) {
        auto& server = std::get<Server>(net);
        _need_update = !server.accept_client();
    }
}

void NetManager::close_connection() {
    _need_update      = false;
    std::visit([](auto&& v) { v.disconnect(); }, net);
}

bool NetManager::is_server() const { return std::holds_alternative<Server>(net); }

bool NetManager::is_client() const { return std::holds_alternative<Client>(net); }

bool NetManager::is_done() const { return !_need_update; }
