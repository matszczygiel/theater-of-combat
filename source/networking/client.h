#pragma once

#include <SFML/Network.hpp>
#include <TGUI/TGUI.hpp>

class Client {
   public:
    Client() = delete;

    static void connect_to_server(const sf::IpAddress& ip, const unsigned short& port);
    static void disconnect();

    static tgui::ChildWindow::Ptr create_prompt_window();

   private:
    static sf::TcpSocket _socket;
};