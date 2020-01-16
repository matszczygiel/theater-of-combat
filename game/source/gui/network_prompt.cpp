#include "network_prompt.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <SFML/Network/IpAddress.hpp>

#include "networking/net_manager.h"

void show_network_prompt(std::variant<Server, Client>& net, std::string title,
                         bool* open) {
    if (!ImGui::Begin(title.c_str(), open)) {
        ImGui::End();
        return;
    }

    static int current_item = 0;
    static std::string ip{};
    static const sf::IpAddress ip_local = sf::IpAddress::getLocalAddress();
    static sf::IpAddress ip_remote{};
    static bool server_accepting = false;
    ImGui::Combo("Connection type", &current_item, "Server\0Client\0");
    if (current_item == 0) {
        if (!std::holds_alternative<Server>(net)) {
            net.emplace<Server>();
        }

        auto& server = std::get<Server>(net);

        if (server_accepting) {
            if (server.accept_client()) {
                ip_remote        = server.get_remote_ip();
                server_accepting = false;
            }
            if (ImGui::Button("Stop listening for clients")) {
                server_accepting = false;
            }
        } else {
            if (ImGui::Button("Listen for clients")) {
                server.listen(NetManager::port);
                server_accepting = true;
            }
        }
    } else if (current_item == 1) {
        ImGui::InputText("Ip", &ip);
        server_accepting = false;
        if (ImGui::Button("Connect")) {
            auto& client = net.emplace<Client>();
            if (client.connect(sf::IpAddress{ip}, NetManager::port)) {
                ip_remote  = client.get_remote_ip();
            }
        }
    }
    ImGui::BulletText("Local Ip :  %s\n"
                      "Remote Ip:  %s\n",
                      ip_local.toString().c_str(), ip_remote.toString().c_str());

    ImGui::End();
}
