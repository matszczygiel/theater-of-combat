#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H

#include <sstream>
#include <string>
#include <vector>

#include <spdlog/sinks/ostream_sink.h>

class LogWindow {
   public:
    LogWindow();
    ~LogWindow();

    void clear();
    void show_window(bool* p_open = nullptr);

   private:
    void draw(std::string title, bool* open = nullptr);
    void read_sink();

    std::vector<std::string> _lines_buffer{};
    std::stringstream _ss{};
    spdlog::sink_ptr _sink{new spdlog::sinks::ostream_sink_mt(_ss)};
};

#endif