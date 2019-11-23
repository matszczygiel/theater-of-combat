#ifndef MENU_BAR_H
#define MENU_BAR_H

struct MenuOptions {
    bool show_dock_space{true};
    bool show_log_console{true};
    bool show_console{true};
    bool show_system_select{false};
    bool show_map_creator{false};
    bool show_imgui_demo{false};
};

void show_menu_bar(MenuOptions& options);

#endif