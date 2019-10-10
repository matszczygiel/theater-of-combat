#ifndef LOG_H
#define LOG_H

#include <cassert>
#include <memory>

#include <spdlog/fmt/ostr.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/dist_sink.h>

namespace logger {
std::shared_ptr<spdlog::sinks::dist_sink_mt>& get_distributing_sink() noexcept;
spdlog::logger& get_app_logger() noexcept;
spdlog::logger& get_engine_logger() noexcept;
}  // namespace logger

template <typename... Args>
inline void engine_trace(const char* fmt, const Args&... args) {
    logger::get_engine_logger().trace(fmt, args...);
}

template <typename... Args>
inline void engine_debug(const char* fmt, const Args&... args) {
    logger::get_engine_logger().debug(fmt, args...);
}

template <typename... Args>
inline void engine_info(const char* fmt, const Args&... args) {
    logger::get_engine_logger().info(fmt, args...);
}

template <typename... Args>
inline void engine_warn(const char* fmt, const Args&... args) {
    logger::get_engine_logger().warn(fmt, args...);
}

template <typename... Args>
inline void engine_error(const char* fmt, const Args&... args) {
    logger::get_engine_logger().error(fmt, args...);
}

template <typename... Args>
inline void engine_critical(const char* fmt, const Args&... args) {
    logger::get_engine_logger().critical(fmt, args...);
}

template <typename... Args>
inline void app_trace(const char* fmt, const Args&... args) {
    logger::get_app_logger().trace(fmt, args...);
}

template <typename... Args>
inline void app_debug(const char* fmt, const Args&... args) {
    logger::get_app_logger().debug(fmt, args...);
}

template <typename... Args>
inline void app_info(const char* fmt, const Args&... args) {
    logger::get_app_logger().info(fmt, args...);
}

template <typename... Args>
inline void app_warn(const char* fmt, const Args&... args) {
    logger::get_app_logger().warn(fmt, args...);
}

template <typename... Args>
inline void app_error(const char* fmt, const Args&... args) {
    logger::get_app_logger().error(fmt, args...);
}

template <typename... Args>
inline void app_critical(const char* fmt, const Args&... args) {
    logger::get_app_logger().critical(fmt, args...);
}

template <typename... Args>
inline void engine_assert_throw(bool condition, const char* message,
                                const Args&... args) {
    if (!condition) {
        engine_critical(message, args...);
        throw std::runtime_error("Paused on engine_throw. Check logs for more info.");
    }
}

template <typename... Args>
inline void app_assert_throw(bool condition, const char* message, const Args&... args) {
    if (!condition) {
        app_critical(message, args...);
        throw std::runtime_error("Paused on app_throw. Check logs for more info.");
    }
}

template <typename... Args>
inline void engine_assert(bool condition, const char* message, const Args&... args) {
    if (!condition) {
        engine_critical(message, args...);
        assert(condition);
    }
}

template <typename... Args>
inline void app_assert(bool condition, const char* message, const Args&... args) {
    if (!condition) {
        app_critical(message, args...);
        assert(condition);
    }
}

#endif