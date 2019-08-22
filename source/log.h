#ifndef LOG_H
#define LOG_H

#include <memory>
#include <string>

#include <spdlog/logger.h>
#include <spdlog/sinks/dist_sink.h>

namespace logger {
std::shared_ptr<spdlog::sinks::dist_sink_mt>& get_distributing_sink();
std::unique_ptr<spdlog::logger>& get_game_logger();
std::unique_ptr<spdlog::logger>& get_engine_logger();
}  // namespace logger

template <typename... Args>
inline void engine_trace(const char* fmt, const Args&... args) {
    logger::get_engine_logger()->trace(fmt, args...);
}

template <typename... Args>
inline void engine_info(const char* fmt, const Args&... args) {
    logger::get_engine_logger()->info(fmt, args...);
}

template <typename... Args>
inline void engine_warn(const char* fmt, const Args&... args) {
    logger::get_engine_logger()->warn(fmt, args...);
}

template <typename... Args>
inline void engine_error(const char* fmt, const Args&... args) {
    logger::get_engine_logger()->error(fmt, args...);
}

template <typename... Args>
inline void engine_critical(const char* fmt, const Args&... args) {
    logger::get_engine_logger()->critical(fmt, args...);
}

template <typename... Args>
inline void game_trace(const char* fmt, const Args&... args) {
    logger::get_game_logger()->trace(fmt, args...);
}

template <typename... Args>
inline void game_info(const char* fmt, const Args&... args) {
    logger::get_game_logger()->info(fmt, args...);
}

template <typename... Args>
inline void game_warn(const char* fmt, const Args&... args) {
    logger::get_game_logger()->warn(fmt, args...);
}

template <typename... Args>
inline void game_error(const char* fmt, const Args&... args) {
    logger::get_game_logger()->error(fmt, args...);
}

template <typename... Args>
inline void game_critical(const char* fmt, const Args&... args) {
    logger::get_game_logger()->critical(fmt, args...);
}

#endif