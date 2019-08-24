#include "log_window.h"

#include <imgui.h>

#include "log.h"

LogWindow::LogWindow() {
    _sink->set_pattern("%^[%T] %n [%l]: %v%$");
    _sink->set_level(spdlog::level::trace);
    logger::get_distributing_sink()->add_sink(_sink);
}

LogWindow::~LogWindow() { logger::get_distributing_sink()->remove_sink(_sink); }

void LogWindow::clear() { _oss.str("");  }

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
    // The simplest and easy way to display the entire buffer:
    const auto oss_string = _oss.str();
    ImGui::TextUnformatted(&oss_string.front(), &oss_string.back());
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
    /*ImGuiListClipper clipper;
    clipper.Begin(LineOffsets.Size);
    while (clipper.Step()) {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd;
             line_no++) {
            const char* line_start = buf + LineOffsets[line_no];
            const char* line_end   = (line_no + 1 < LineOffsets.Size)
                                       ? (buf + LineOffsets[line_no + 1] - 1)
                                       : buf_end;
            ImGui::TextUnformatted(line_start, line_end);
        }
    }
    clipper.End();
*/
    ImGui::PopStyleVar();

    if (_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
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
        static int counter = 0;
        for (int n = 0; n < 5; n++) {
            const char* categories[3] = {"info", "warn", "error"};
            const char* words[]       = {
                "Bumfuzzled",   "Cattywampus", "Snickersnee", "Abibliophobia",
                "Absquatulate", "Nincompoop",  "Pauciloquent"};
            std::string msg =
                "[" + std::to_string(ImGui::GetFrameCount()) +
                "]"
                "[" +
                std::string(categories[counter % IM_ARRAYSIZE(categories)]) +
                "] Hello, current time is" + std::to_string(ImGui::GetTime()) +
                ", here's a word: '" +
                std::string(words[counter % IM_ARRAYSIZE(words)]) + "'\n";
            _oss << msg;
            ++counter;
        }
    }
    ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same
    // window as we just did)
    draw("Log console", p_open);
}
