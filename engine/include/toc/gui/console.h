#ifndef CONSOLE_H   
#define CONSOLE_H

#include <vector>
#include <string>
#include <optional>

#include <imgui.h>

class ConsoleWindow {
    public:
     explicit ConsoleWindow(std::string name);

     void show(bool* p_open = nullptr);

    private:
     enum class ItemType { command, error, output };

     void execute_command(std::string cmd);
     void clear();
     void add_item(ItemType type, std::string line);
     void clear_buffer();

     std::string _in_buffer{};
     std::vector<std::pair<ItemType, std::string>> _items{};
     std::vector<std::string> _history{};
     int _history_position{-1}; //-1 means no history to display
     ImGuiTextFilter _filter{};
     bool _auto_scroll{true};
     bool _scroll_to_bottom{false};

     std::string _name{};
};

#endif