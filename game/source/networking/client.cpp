#include "client.h"

#include "core/log.h"

bool Client::connect_to_server(const sf::IpAddress& ip,
                               const unsigned short& port) {
    engine_info("Conncecting to server, ip: {0}, port: {1}", ip.toString(),
                port);

    const auto timeout = sf::milliseconds(1000);
    _socket.setBlocking(true);

    if (_socket.connect(ip, port, timeout) != sf::Socket::Status::Done) {
        engine_warn("Connection failed, with timeout: {0} ms",
                    timeout.asMilliseconds());
        return false;
    } else {
        engine_info("Connected.");
        _socket.setBlocking(false);
        return true;
    }
}

void Client::disconnect() { _socket.disconnect(); }

sf::IpAddress Client::get_remote_ip() const {
    return _socket.getRemoteAddress();
}

unsigned short Client::get_remote_port() const {
    return _socket.getRemotePort();
}

bool Client::send(sf::Packet& packet) {
    if (auto status = _socket.send(packet); status != sf::Socket::Done) {
        if (status != sf::Socket::Partial)
            return false;
        while (_socket.send(packet) == sf::Socket::Partial)
            ;
        return true;
    }
    return true;
}

bool Client::receive(sf::Packet& packet) {
    if (auto status = _socket.receive(packet); status != sf::Socket::Done) {
        if (status != sf::Socket::Partial)
            return false;
        while (_socket.receive(packet) == sf::Socket::Partial)
            ;
        return true;
    }
    return true;
}
