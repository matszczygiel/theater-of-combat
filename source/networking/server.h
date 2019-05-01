#pragma once

#include <SFML/Network.hpp>
#include <TGUI/TGUI.hpp>

class Server {
   public:
    static void listen_at_port(const unsigned short& port);
    static void accept_client();

    static sf::IpAddress get_local_ip();
    static sf::IpAddress get_public_ip();
    static unsigned short get_port();

    static tgui::ChildWindow::Ptr create_prompt_window();

   private:
    static sf::TcpSocket _socket;
    static sf::TcpListener _listener;
};