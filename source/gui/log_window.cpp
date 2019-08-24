#include "log_window.h"

#include <imgui.h>

#include "log.h"

LogWindow::LogWindow() {
    _sink->set_pattern("%^[%T] %n [%l]: %v%$");
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
    static bool auto_scroll{true};
    if (!ImGui::Begin(title.c_str(), open)) {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &auto_scroll);
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
    // The simplest and easy way to display the entire buffer:
    // ImGui::TextUnformatted(&_buffer.front(), &_buffer.back());
    // And it'll just work. TextUnformatted() has specialization for large
    // blob of text and will fast-forward to skip non-visible lines. Here we
    // instead demonstrate using the clipper to only process lines that are
    // within the visible area. If you have tens of thousands of items and
    // their processing cost is non-negligible, coarse clipping them on your
    // side is recommended. Using ImGuiListClipper requires A) random access
    // into your data, and B) items all being the  same height, both of
    // which we can handle since we an array pointing to the beginning of
    // each line of text. When using the filter (in the block of code above)
    // we don't have random access into the data to display anymore, which
    // is why we don't use the clipper. Storing or skimming through the
    // search result would make it possible (and would be recommended if you
    // want to search through tens of thousands of entries)
    static ImGuiTextFilter filter;
    filter.Draw("Filter", -100.0f);

    ImGuiListClipper clipper;
    if (filter.IsActive()) {
        // In this example we don't use the clipper when Filter is enabled.
        // This is because we don't have a random access on the result on our
        // filter. A real application processing logs with ten of thousands of
        // entries may want to store the result of search/filter. especially if
        // the filtering function is not trivial (e.g. reg-exp).
        for (const auto& line : _lines_buffer) {
            if (filter.PassFilter(&line.front(), &line.back()))
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

    if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}

// Demonstrate creating a simple log window with basic filtering.
void LogWindow::show_window(bool* p_open) {
    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to
    // Begin()/End() are appending to the _same_ window. Most of the contents of
    // the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Log console", p_open);
    if (ImGui::SmallButton("[Debug] Add 5 entries")) {
        engine_critical("Test no: {} message to test Log console", 1);
        engine_error("Test no: {} message to test Log console", 2);
        engine_info("Test no: {} message to test Log console", 3);
        engine_debug("Test no: {} message to test Log console", 4);
        engine_trace("Test no: {} message to test Log console", 5);
    }
    ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same
    // window as we just did)
    draw("Log console", p_open);
}
