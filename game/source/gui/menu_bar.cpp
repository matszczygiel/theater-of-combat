#include "menu_bar.h"

#include <memory>

#include <imgui.h>

static void show_options(MenuOptions& options) {
    ImGui::Checkbox("Dock space", &options.show_dock_space);
    ImGui::Checkbox("Lua console", &options.show_console);
    ImGui::Checkbox("Log console", &options.show_log_console);
    ImGui::Checkbox("Network prompt", &options.show_network_prompt);
    ImGui::Checkbox("ImGui demo", &options.show_imgui_demo);
}

void show_menu_bar(MenuOptions& options) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            show_options(options);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}