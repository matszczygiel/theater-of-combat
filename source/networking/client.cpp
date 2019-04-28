#include "client.h"

#include "log.h"

sf::TcpSocket Client::_socket;

void Client::connect_to_server(const sf::IpAddress& ip, const unsigned short& port) {
    ENGINE_INFO("Conncecting to server, ip: {0}, port: {1}", ip.toString(), port);

    const auto timeout = sf::milliseconds(1000);

    if (_socket.connect(ip, port, timeout) != sf::Socket::Status::Done) {
        ENGINE_WARN("Connection failed, with timeout: {0} ms", timeout.asMilliseconds());
    }
}

void Client::disconnect() {
    _socket.disconnect();
}