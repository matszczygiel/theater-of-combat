#include "server.h"

#include "log.h"

bool Server::listen_at_port(const unsigned short& port) {
    _listener.setBlocking(false);
    ENGINE_INFO("Server listening port: {0}", port);
    if (_listener.listen(port) != sf::Socket::Status::Done) {
        ENGINE_WARN("Listener failed.");
        return false;
    }
    return true;
}

bool Server::accept_client() {
    if (_listener.accept(_socket) != sf::Socket::Status::Done) {
        return false;
    }
    ENGINE_INFO("Server accepted client.");
    return true;
}

sf::IpAddress Server::get_local_ip() {
    return sf::IpAddress::getLocalAddress();
}
unsigned short Server::get_port() { return _listener.getLocalPort(); }

sf::IpAddress Server::get_public_ip() {
    return sf::IpAddress::getPublicAddress();
}
