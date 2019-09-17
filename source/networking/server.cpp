#include "server.h"

#include "core/log.h"

bool Server::listen_at_port(const unsigned short& port) {
    engine_info("Server listening port: {0}", port);
    if (_listener.listen(port) != sf::Socket::Status::Done) {
        engine_warn("Listener failed.");
        return false;
    }
    return true;
}

bool Server::accept_client() {
    if (_listener.accept(_socket) != sf::Socket::Status::Done) {
        return false;
    }
    engine_info("Server accepted client: {}.",
                _socket.getRemoteAddress().toString());
    return true;
}

sf::IpAddress Server::get_local_ip() {
    return sf::IpAddress::getLocalAddress();
}
unsigned short Server::get_port() { return _listener.getLocalPort(); }

sf::IpAddress Server::get_public_ip() {
    return sf::IpAddress::getPublicAddress();
}

sf::IpAddress Server::get_remote_ip() const {
    return _socket.getRemoteAddress();
}
