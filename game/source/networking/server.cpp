#include "server.h"

#include "toc/core/log.h"

bool Server::listen_at_port(const unsigned short& port) {
    engine_info("Server listening port: {0}", port);
    _listener.setBlocking(true);
    if (_listener.listen(port) != sf::Socket::Status::Done) {
        engine_warn("Listener failed.");
        return false;
    }
    engine_info("Server bound to port: {}", get_port());
    _listener.setBlocking(false);
    return true;
}

bool Server::accept_client() {
    if (_listener.accept(_socket) == sf::Socket::Done) {
        _listener.close();
        engine_info("Server accepted client: {}.", _socket.getRemoteAddress().toString());
        _socket.setBlocking(false);
        return true;
    }
    return false;
}

sf::IpAddress Server::get_local_ip() { return sf::IpAddress::getLocalAddress(); }

unsigned short Server::get_port() { return _listener.getLocalPort(); }

sf::IpAddress Server::get_public_ip() { return sf::IpAddress::getPublicAddress(); }

sf::IpAddress Server::get_remote_ip() const { return _socket.getRemoteAddress(); }

bool Server::send(sf::Packet& packet) {
    if (auto status = _socket.send(packet); status != sf::Socket::Done) {
        if (status != sf::Socket::Partial)
            return false;
        while (_socket.send(packet) == sf::Socket::Partial)
            ;
        return true;
    }
    return true;
}

bool Server::receive(sf::Packet& packet) {
    if (auto status = _socket.receive(packet); status != sf::Socket::Done) {
        if (status != sf::Socket::Partial)
            return false;
        while (_socket.receive(packet) == sf::Socket::Partial)
            ;
        return true;
    }
    return true;
}
