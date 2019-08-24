#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H

#include <memory>
#include <sstream>
#include <string>

#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/ostream_sink.h>

class LogWindow {
   public:
    explicit LogWindow();
    ~LogWindow();

    void clear();

    void show_window(bool* p_open = nullptr);

   private:
    void draw(std::string title, bool* open = nullptr);

    bool _auto_scroll{true};
    std::ostringstream _oss{};
    spdlog::sink_ptr _sink{new spdlog::sinks::ostream_sink_mt(_oss)};
};

#endif