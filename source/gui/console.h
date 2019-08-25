#ifndef CONSOLE_H   
#define CONSOLE_H

#include <vector>
#include <string>

#include <imgui.h>

class ConsoleWindow {
    public:
     ConsoleWindow(std::string name);

     void show(bool* p_open = nullptr);

    private:
     void execute_command(std::string cmd);
     void clear();
     void add_line(std::string line);
     void clear_buffer();

     std::string _in_buffer{};
     std::vector<std::string> _lines{};
     std::vector<std::string> _history{};
     ImGuiTextFilter _filter{};
     bool _auto_scroll{true};
     bool _scroll_to_bottom{false};

     std::string _name{};
};

#endif