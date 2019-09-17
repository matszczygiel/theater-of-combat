#include "network_prompt.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <SFML/Network/IpAddress.hpp>

void show_network_prompt(std::variant<Server, Client>& net, std::string title,
                         bool* open) {
    if (!ImGui::Begin(title.c_str(), open)) {
        ImGui::End();
        return;
    }

    static int current_item = 0;
    static int port         = 0;
    static std::string ip{};
    static sf::IpAddress ip_local = sf::IpAddress::getLocalAddress();
    static sf::IpAddress ip_remote{};
    static unsigned short bound_port = 0;
    ImGui::Combo("Connection type", &current_item, "Server\0Client\0");
    if (current_item == 0) {
        ImGui::InputInt("Port", &port);
        if (ImGui::Button("Bind to port")) {
            auto& server = net.emplace<Server>();
            server.listen_at_port(port);
            bound_port = server.get_port();
        }
        if (ImGui::Button("Listen for clients")) {
            if (net.index() == 0) {
                auto& server = std::get<0>(net);
                server.accept_client();
                ip_remote = server.get_remote_ip();
            }
        }
    }
    if (current_item == 1) {
        ImGui::InputInt("Port", &port);
        ImGui::InputText("Ip", &ip);
        if (ImGui::Button("Connect")) {
            auto& client = net.emplace<Client>();
            if (client.connect_to_server(sf::IpAddress{ip}, port)) {
                bound_port = client.get_remote_port();
                ip_remote  = client.get_remote_ip();
            }
        }
    }
    ImGui::BulletText("Local Ip :  %s\n"
                      "Remote Ip:  %s\n"
                      "Port        %d\n",
                      ip_local.toString().c_str(), ip_remote.toString().c_str(),
                      bound_port);

    ImGui::End();
}
