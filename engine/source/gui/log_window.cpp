#include "gui/log_window.h"

#include <imgui.h>

#include "core/log.h"

LogWindow::LogWindow(std::string name) : _name(name) {
    _sink->set_pattern("%^[%T] %n: [%l] %v%$");
    _sink->set_level(spdlog::level::trace);
    logger::get_distributing_sink()->add_sink(_sink);
}

LogWindow::~LogWindow() { logger::get_distributing_sink()->remove_sink(_sink); }

void LogWindow::clear() { _lines_buffer.clear(); }

void LogWindow::read_sink() {
    std::string line{};
    while (std::getline(_ss, line)) {
        _lines_buffer.push_back(line + '\n');
    }
    _ss.clear();
}

void LogWindow::draw(std::string title, bool* open) {
    if (!ImGui::Begin(title.c_str(), open)) {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &_auto_scroll);
        ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

    if (clear)
        this->clear();
    if (copy)
        ImGui::LogToClipboard();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    read_sink();
    _filter.Draw("Filter", -100.0f);

    ImGuiListClipper clipper;
    if (_filter.IsActive()) {
        for (const auto& line : _lines_buffer) {
            if (_filter.PassFilter(&line.front(), &line.back()))
                ImGui::TextUnformatted(&line.front(), &line.back());
        }
    } else {
        clipper.Begin(_lines_buffer.size());
        while (clipper.Step()) {
            for (int line_no = clipper.DisplayStart;
                 line_no < clipper.DisplayEnd; ++line_no) {
                ImGui::TextUnformatted(&_lines_buffer[line_no].front(),
                                       &_lines_buffer[line_no].back());
            }
        }
        clipper.End();
    }
    ImGui::PopStyleVar();

    if (_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}

void LogWindow::show_window(bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin(_name.c_str(), p_open);
    if (ImGui::SmallButton("[Debug] Add 5 entries")) {
        engine_critical("Test no: {} message to test Log console", 1);
        engine_error("Test no: {} message to test Log console", 2);
        engine_info("Test no: {} message to test Log console", 3);
        engine_debug("Test no: {} message to test Log console", 4);
        engine_trace("Test no: {} message to test Log console", 5);
    }
    ImGui::End();
    draw(_name.c_str(), p_open);
}
