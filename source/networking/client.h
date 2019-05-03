#pragma once

#include <SFML/Network.hpp>
#include <TGUI/TGUI.hpp>

#include "netwoking_status.h"

class Client {
   public:
    bool connect_to_server(const sf::IpAddress& ip, const unsigned short& port);
    void disconnect();

    tgui::ChildWindow::Ptr create_prompt_window(Network_status& status);

   private:
    sf::TcpSocket _socket;
};