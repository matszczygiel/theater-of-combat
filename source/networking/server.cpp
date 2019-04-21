#include "server.h"

#include "log.h"

void Server::listen_at_port(const unsigned short& port) {
    ENGINE_INFO("Server listening port: {0}", port);
    if (_listener.listen(port) != sf::Socket::Status::Done) {
        ENGINE_WARN("Listener failed.");
    }
}

void Server::accept_client() {
    ENGINE_INFO("Server is accepting client");
    if (_listener.accept(_socket) != sf::Socket::Status::Done) {
        ENGINE_WARN("Failed to accept client.");
    }
}

sf::IpAddress Server::get_local_ip() const {
    return sf::IpAddress::getLocalAddress();
}
unsigned short Server::get_port() const {
    return _listener.getLocalPort();
}

sf::IpAddress Server::get_public_ip() const {
    return sf::IpAddress::getPublicAddress();
}
