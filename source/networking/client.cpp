#include "client.h"

#include "log.h"

sf::TcpSocket Client::_socket;

void Client::connect_to_server(const sf::IpAddress& ip, const unsigned short& port) {
    ENGINE_INFO("Conncecting to server, ip: {0}, port: {1}", ip.toString(), port);

    const auto timeout = sf::milliseconds(1000);

    if (_socket.connect(ip, port, timeout) != sf::Socket::Status::Done) {
        ENGINE_WARN("Connection failed, with timeout: {0} ms", timeout.asMilliseconds());
    } else {
        ENGINE_INFO("Connected.");
    }
}

void Client::disconnect() {
    _socket.disconnect();
}

tgui::ChildWindow::Ptr Client::create_prompt_window() {
    auto window = tgui::ChildWindow::create("Connect");
    window->setSize("50%", "30%");
    window->setPosition("35%", "35%");

    auto ip_box = tgui::EditBox::create();
    ip_box->setSize("80%", "10%");
    ip_box->setTextSize(18);
    ip_box->setPosition("10%", "20%");
    ip_box->setDefaultText("IP adress");
    window->add(ip_box, "ip box");

    auto port_box = tgui::EditBox::create();
    port_box->setSize("50%", "10%");
    port_box->setTextSize(18);
    port_box->setPosition("25%", "50%");
    port_box->setDefaultText("Port");
    window->add(port_box, "port box");

    auto button = tgui::Button::create();
    button->setSize("50%", "20%");
    button->setPosition("25%", "75%");
    button->setText("Connect");

    button->connect("pressed", [](tgui::EditBox::Ptr port_b, tgui::EditBox::Ptr ip_b, tgui::Button::Ptr this_button) {
        std::string port_str = port_b->getText().toAnsiString();
        const auto port      = static_cast<unsigned short>(std::stoul(port_str));
        sf::IpAddress ip(ip_b->getText().toAnsiString());

        this_button->setText("Connecting...");

        connect_to_server(ip, port);
    },
                    port_box, ip_box, button);

    window->add(button);

    return window;
}