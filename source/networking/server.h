#pragma once

#include <SFML/Network.hpp>
#include <TGUI/TGUI.hpp>

#include "netwoking_status.h"

class Server {
   public:
    bool listen_at_port(const unsigned short& port);
    bool accept_client();

    static sf::IpAddress get_local_ip();
    static sf::IpAddress get_public_ip();
    unsigned short get_port();

    tgui::ChildWindow::Ptr create_prompt_window(Network_status& status);

   private:
    sf::TcpSocket _socket;
    sf::TcpListener _listener;
};