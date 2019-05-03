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
unsigned short Server::get_port() {
    return _listener.getLocalPort();
}

sf::IpAddress Server::get_public_ip() {
    return sf::IpAddress::getPublicAddress();
}

tgui::ChildWindow::Ptr Server::create_prompt_window(Network_status& status) {
    auto window = tgui::ChildWindow::create("Server");
    window->setSize("50%", "30%");
    window->setPosition("35%", "35%");

    auto label_loc = tgui::Label::create();
    auto label_pub = tgui::Label::create();
    label_loc->setText("Local IP:  " + Server::get_local_ip().toString());
    label_pub->setText("Public IP: " + Server::get_public_ip().toString());
    label_loc->setPosition("5%", "15%");
    label_pub->setPosition("5%", "25%");
    label_loc->setTextSize(18);
    label_pub->setTextSize(18);
    window->add(label_loc);
    window->add(label_pub);

    auto edit_box = tgui::EditBox::create();
    edit_box->setSize("50%", "10%");
    edit_box->setTextSize(18);
    edit_box->setPosition("25%", "50%");
    edit_box->setDefaultText("Port");
    window->add(edit_box, "port box");

    auto button = tgui::Button::create();
    button->setSize("50%", "20%");
    button->setPosition("25%", "75%");
    button->setText("Create");

    button->connect("pressed", [&](tgui::EditBox::Ptr port_box, tgui::Button::Ptr this_button, tgui::ChildWindow::Ptr parent_window) {
        std::string port_str = port_box->getText().toAnsiString();
        const auto port      = static_cast<unsigned short>(std::stoul(port_str));
        if (listen_at_port(port)) {
            this_button->setText("Waiting for player...");
            status = Network_status::sever_accepting;
            parent_window->destroy();
        } else {
            port_box->setText("Invalid port");
        }
    },
                    edit_box, button, window);

    window->add(button);

    return window;
}