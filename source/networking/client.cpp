#include "client.h"

#include "../log.h"

bool Client::connect_to_server(const sf::IpAddress& ip,
                               const unsigned short& port) {
    ENGINE_INFO("Conncecting to server, ip: {0}, port: {1}", ip.toString(),
                port);

    const auto timeout = sf::milliseconds(1000);
    _socket.setBlocking(true);

    if (_socket.connect(ip, port, timeout) != sf::Socket::Status::Done) {
        ENGINE_WARN("Connection failed, with timeout: {0} ms",
                    timeout.asMilliseconds());
        return false;
    } else {
        ENGINE_INFO("Connected.");
        return true;
    }
}

void Client::disconnect() { _socket.disconnect(); }