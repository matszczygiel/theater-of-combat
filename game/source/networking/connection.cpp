#include "connection.h"

#include <toc/core/log.h>

sf::IpAddress Messenger::get_remote_ip() const { return _socket.getRemoteAddress(); }

unsigned short Messenger::get_remote_port() const { return _socket.getRemotePort(); }

bool Messenger::send(sf::Packet& packet) {
    if (const auto status = _socket.send(packet); status != sf::Socket::Done) {
        if (status != sf::Socket::Partial)
            return false;
        while (_socket.send(packet) == sf::Socket::Partial)
            ;
        return true;
    }
    return true;
}

bool Messenger::receive(sf::Packet& packet) {
    if (const auto status = _socket.receive(packet); status != sf::Socket::Done) {
        if (status != sf::Socket::Partial)
            return false;
        while (_socket.receive(packet) == sf::Socket::Partial)
            ;
        return true;
    }
    return true;
}

void Messenger::disconnect() { _socket.disconnect(); }

bool Server::listen(const unsigned short& port) {
    engine_info("Server listening port: {0}", port);
    _listener.setBlocking(true);
    if (_listener.listen(port) != sf::Socket::Status::Done) {
        engine_warn("Listener failed.");
        return false;
    }
    engine_info("Server bound: {}", _listener.getLocalPort());
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

bool Client::connect(const sf::IpAddress& ip, const unsigned short& port) {
    engine_info("Connecting to server, ip: {0}, port: {1}", ip.toString(), port);

    const auto timeout = sf::milliseconds(1000);
    _socket.setBlocking(true);

    if (_socket.connect(ip, port, timeout) != sf::Socket::Status::Done) {
        engine_warn("Connection failed, with timeout: {0} ms", timeout.asMilliseconds());
        return false;
    } else {
        engine_info("Connected.");
        _socket.setBlocking(false);
        return true;
    }
}
