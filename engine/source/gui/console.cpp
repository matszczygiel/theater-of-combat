#include "gui/console.h"

#include <algorithm>

#include <lua.hpp>

#include "core/log.h"
#include "core/lua_vm.h"

using namespace std::string_literals;

ConsoleWindow::ConsoleWindow(std::string name) : _name{std::move(name)} {
    clear_buffer();
    add_item(ItemType::output, "Welcome to " + _name);
    auto& lua = lua::get_state();
    lua.open_libraries(sol::lib::base);
    lua.set_function("print", [&](const sol::variadic_args& vargs) {
        std::string res;
        for (const auto& arg : vargs) {
            res += lua["tostring"](arg.get<sol::object>());
            res += " ";
        }
        add_item(ItemType::output, res);
        engine_trace("Lua output: {}", res);
        return 0;
    });
}

void ConsoleWindow::add_item(ItemType type, std::string line) {
    switch (type) {
        case ItemType::command:
            _items.emplace_back(type, "> " + line + "\n");
            break;
        case ItemType::output:
            _items.emplace_back(type, line + "\n");
            break;
        case ItemType::error:
            _items.emplace_back(type, "[error]: " + line + "\n");
            break;
    }
}

void ConsoleWindow::clear() {
    _items.clear();
    _history.clear();
}

void ConsoleWindow::clear_buffer() {
    constexpr int buffer_size = 256;
    _in_buffer.clear();
    _in_buffer.resize(buffer_size);
}

void ConsoleWindow::execute_command(std::string cmd) {
    add_item(ItemType::command, cmd);
    engine_trace("Lua command: {}", cmd);
    try {
        lua::get_state().script(cmd, &lua::error_handler);
    } catch (const sol::error& e) {
        add_item(ItemType::error, e.what() + "\n"s);
    }

    _history.erase(std::remove(_history.begin(), _history.end(), cmd),
                   _history.end());
    if (cmd != "")
        _history.push_back(cmd);
    _history_position = -1;

    _scroll_to_bottom = true;
}

void ConsoleWindow::show(bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(_name.c_str(), p_open)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginPopupContextItem()) {
        const auto menu_title = "Close " + _name;
        if (ImGui::MenuItem(menu_title.c_str()))
            *p_open = false;
        ImGui::EndPopup();
    }

    if (ImGui::SmallButton("Clear")) {
        clear();
    }
    ImGui::SameLine();
    bool copy_to_clipboard = ImGui::SmallButton("Copy");

    ImGui::Separator();

    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &_auto_scroll);
        ImGui::EndPopup();
    }

    // Options, Filter
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    _filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
    ImGui::Separator();

    const float footer_height_to_reserve =
        ImGui::GetStyle().ItemSpacing.y +
        ImGui::GetFrameHeightWithSpacing();  // 1 separator, 1 input text

    ImGui::BeginChild(
        "ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false,
        ImGuiWindowFlags_HorizontalScrollbar);  // Leave room for 1
                                                // separator + 1 InputText
    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Clear"))
            clear();
        ImGui::EndPopup();
    }

    // Display every line as a separate entry so we can change their color
    // or add custom widgets. If you only want raw text you can use
    // ImGui::TextUnformatted(log.begin(), log.end()); NB- if you have
    // thousands of entries this approach may be too inefficient and may
    // require user-side clipping to only process visible items. You can
    // seek and display only the lines that are visible using the
    // ImGuiListClipper helper, if your elements are evenly spaced and you
    // have cheap random access to the elements. To use the clipper we could
    // replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
    //     ImGuiListClipper clipper(Items.Size);
    //     while (clipper.Step())
    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd;
    //         i++)
    // However, note that you can not use this code as is if a filter is
    // active because it breaks the 'cheap random-access' property. We would
    // need random-access on the post-filtered list. A typical application
    // wanting coarse clipping and filtering may want to pre-compute an
    // array of indices that passed the filtering test, recomputing this
    // array when user changes the filter, and appending newly elements as
    // they are inserted. This is left as a task to the user until we can
    // manage to improve this example code! If your items are of variable
    // size you may want to implement code similar to what ImGuiListClipper
    // does. Or split your data into fixed height items to allow
    // random-seeking into your list.
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                        ImVec2(4, 1));  // Tighten spacing
    if (copy_to_clipboard)
        ImGui::LogToClipboard();
    for (const auto& [type, item] : _items) {
        if (!_filter.PassFilter(&item.front(), &item.back()))
            continue;

        switch (type) {
            case ItemType::command:
                ImGui::PushStyleColor(ImGuiCol_Text,
                                      ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
                break;
            case ItemType::error:
                ImGui::PushStyleColor(ImGuiCol_Text,
                                      ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                break;
            case ItemType::output:
                ImGui::PushStyleColor(ImGuiCol_Text,
                                      ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                break;
        }
        ImGui::TextUnformatted(&item.front(), &item.back());
        ImGui::PopStyleColor();
    }
    if (copy_to_clipboard)
        ImGui::LogFinish();

    if (_scroll_to_bottom ||
        (_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    _scroll_to_bottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    constexpr auto history_event_callback =
        [](ImGuiInputTextCallbackData* data) {
            assert(data->EventFlag == ImGuiInputTextFlags_CallbackHistory);
            auto console     = static_cast<ConsoleWindow*>(data->UserData);
            const auto& hist = console->_history;
            auto& hist_pos   = console->_history_position;
            const auto prev_history_pos = hist_pos;
            const auto history_size     = static_cast<int>(hist.size());
            if (data->EventKey == ImGuiKey_UpArrow) {
                if (hist_pos == -1)
                    hist_pos = history_size - 1;
                else if (hist_pos > 0)
                    --hist_pos;
            } else if (data->EventKey == ImGuiKey_DownArrow) {
                if (hist_pos != -1)
                    if (++hist_pos >= history_size)
                        hist_pos = -1;
            }

            // A better implementation would preserve the data on the
            // current input line along with cursor position.
            if (prev_history_pos != hist_pos) {
                const auto history_str = (hist_pos >= 0) ? hist[hist_pos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str.c_str());
            }
            return 0;
        };

    // Command-line
    bool reclaim_focus = false;
    if (ImGui::InputText("Input", &_in_buffer.front(), _in_buffer.size(),
                         ImGuiInputTextFlags_EnterReturnsTrue |
                             ImGuiInputTextFlags_CallbackHistory,
                         history_event_callback, static_cast<void*>(this))) {
        _in_buffer.erase(std::find(_in_buffer.begin(), _in_buffer.end(), '\0'),
                         _in_buffer.end());
        _in_buffer.erase(0, _in_buffer.find_first_not_of(" \n\r\t"));
        _in_buffer.erase(_in_buffer.find_last_not_of(" \n\r\t") + 1);

        if (!_in_buffer.empty()) {
            execute_command(_in_buffer);
            
        }
        reclaim_focus = true;
        clear_buffer();
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget

    ImGui::End();
}
