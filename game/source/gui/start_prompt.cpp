#include "start_prompt.h"

#include <imgui.h>
#include <imgui_stdlib.h>

void StartPrompt::show_window() {
    if (!show)
        return;

    constexpr auto window_flags = ImGuiWindowFlags_NoCollapse |
                                  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar |
                                  ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::OpenPopup("start window");
    if (ImGui::BeginPopupModal("start window", nullptr, window_flags)) {
        show_setup_page();

        ImGui::EndPopup();
    }
}

void StartPrompt::show_setup_page() {
    ImGui::Text("Enter your name");
    ImGui::InputText("", &player_name);

    ImGui::Separator();
    ImGui::Text("Select connection type");
    static int current_item;
    ImGui::Combo("", &current_item, "Server\0Client\0");

    ImGui::Separator();
    ImGui::Button("Done", ImVec2(30, 30));
}
