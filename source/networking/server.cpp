#include "server.h"

#include "log.h"

sf::TcpSocket Server::_socket;
sf::TcpListener Server::_listener;

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

sf::IpAddress Server::get_local_ip() {
    return sf::IpAddress::getLocalAddress();
}
unsigned short Server::get_port() {
    return _listener.getLocalPort();
}

sf::IpAddress Server::get_public_ip() {
    return sf::IpAddress::getPublicAddress();
}
