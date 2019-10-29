#include "start_prompt.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "toc/core/log.h"

#include "gameplay/game_state.h"

StartPrompt::StartPrompt(std::shared_ptr<NetManager>& ptr,
                         std::shared_ptr<ResourceLoader>& loader) noexcept
    : _nm{ptr}, _rl{loader} {
    engine_assert(ptr != nullptr, "Cannot initialize StartPrompt with nullptr");
    engine_assert(loader != nullptr, "Cannot initialize StartPrompt with nullptr");
}

void StartPrompt::show_window() {
    if (_stage == Stage::none)
        return;

    constexpr auto window_flags = ImGuiWindowFlags_NoCollapse |
                                  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar |
                                  ImGuiWindowFlags_AlwaysAutoResize |
                                  ImGuiWindowFlags_Modal;

    if (!ImGui::Begin("start window", nullptr, window_flags)) {
        ImGui::End();
        return;
    }
    switch (_stage) {
        case Stage::setup:
            show_setup_page();
            break;
        case Stage::waiting:
            show_waiting_page();
            break;
        case Stage::selecting:
            show_selecting_page();
            break;

        default:
            break;
    }
    ImGui::End();

    // Debug
    if (!ImGui::Begin("start window debug", nullptr)) {
        ImGui::End();
        return;
    }

    if (ImGui::Selectable("none", _stage == Stage::none))
        _stage = Stage::none;
    if (ImGui::Selectable("setup", _stage == Stage::setup))
        _stage = Stage::setup;
    if (ImGui::Selectable("waiting", _stage == Stage::waiting))
        _stage = Stage::waiting;
    if (ImGui::Selectable("selecting", _stage == Stage::selecting))
        _stage = Stage::selecting;

    ImGui::End();
}

void StartPrompt::show_setup_page() {
    ImGui::Text("Enter your name");
    ImGui::InputText("Player name", &_player_name);

    ImGui::Separator();
    ImGui::Text("Select connection type");
    static int current_item{0};
    ImGui::Combo("Connection type", &current_item, "Server\0Client\0");
    if (current_item == 1) {
        ImGui::InputText("Ip", &_server_ip);
        ImGui::InputInt("Port", &_server_port, 0);
    }

    ImGui::Separator();
    if (ImGui::Button("Done", ImVec2(100, 40))) {
        sf::IpAddress ip{_server_ip};
        bool good{false};
        switch (current_item) {
            case 0:
                good          = _nm->setup_server(ip, _server_port);
                _server_ip    = ip.toString();
                _connection_t = ConnectionType::server;
                break;
            case 1:
                good          = _nm->setup_client(ip, _server_port);
                _connection_t = ConnectionType::client;
                break;
        }
        if (good)
            _stage = Stage::waiting;
    }
    ImGui::SameLine();
    if (ImGui::Button("Close", ImVec2(100, 40)))
        _stage = Stage::none;
}

void StartPrompt::show_waiting_page() {
    switch (_connection_t) {
        case ConnectionType::server:
            ImGui::Text("Waiting for player ...");
            ImGui::Separator();
            ImGui::Text("Port  %d", _server_port);
            ImGui::Text("Ip    %s", _server_ip.c_str());
            break;
        case ConnectionType::client:
            ImGui::Text("Connecting ...");
            break;
        default:
            break;
    }
    if (ImGui::Button("Stop", ImVec2(100, 40)))
        _stage = Stage::setup;

    if (_nm->is_done()) {
        _stage     = Stage::selecting;
        _scenarios = _rl->list_avaliable<Scenario>();
    }
}

void StartPrompt::show_selecting_page() {
    static int current_item{0};
    if (ImGui::ListBoxHeader("Select Scenario", _scenarios.size())) {
        int i{0};
        for (const auto& sc : _scenarios) {
            if (ImGui::Selectable(sc.c_str(), current_item == i))
                current_item = i;
            ++i;
        }
        ImGui::ListBoxFooter();
    }

    if (ImGui::Button("Select", ImVec2(100, 40)))
        _stage = Stage::none;

    ImGui::SameLine();
    if (ImGui::Button("Reload", ImVec2(100, 40)))
        _scenarios = _rl->list_avaliable<Scenario>();

    ImGui::SameLine();
    if (ImGui::Button("Back", ImVec2(100, 40)))
        _stage = Stage::setup;
}
