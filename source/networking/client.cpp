#include "client.h"

#include "log.h"

void Client::connect_to_server(const sf::IpAddress& ip, const unsigned short& port) {
    ENGINE_INFO("Conncecting to server, ip: {0}, port: {1}", ip.toString(), port);

    constexpr sf::Int64 timeout = 1000;

    if (_socket.connect(ip, port, sf::Time(timeout)) != sf::Socket::Status::Done) {
        ENGINE_WARN("Connection failed, with timeout: {0} ms", timeout);
    }
}

void Client::disconnect() {
    _socket.disconnect();
}