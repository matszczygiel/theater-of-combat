#ifndef MENU_BAR_H
#define MENU_BAR_H

struct MenuOptions {
    bool show_dock_space{true};
    bool show_log_console{true};
    bool show_console{true};
    bool show_network_prompt{false};
    bool show_imgui_demo{false};
};

void show_menu_bar(MenuOptions& options);

#endif