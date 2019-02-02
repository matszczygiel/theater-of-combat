#include "log.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> Log::_engine_logger = nullptr;
std::shared_ptr<spdlog::logger> Log::_game_logger   = nullptr;

void Log::init_game_logger() {
    _game_logger = spdlog::stdout_logger_mt("game");
    _game_logger->set_pattern("%^[%T] GAME: %v%$");
    _game_logger->set_level(spdlog::level::trace);
}

void Log::init_engine_logger() {
    _engine_logger = spdlog::stdout_color_mt("engine");
    _engine_logger->set_pattern("%^[%T] [thread %t] %v%$");
    _engine_logger->set_level(spdlog::level::trace);
}


std::shared_ptr<spdlog::logger> Log::get_engine_logger() {
    if (_engine_logger == nullptr)
        init_engine_logger();

    return _engine_logger;
}

std::shared_ptr<spdlog::logger> Log::get_game_logger() {
    if (_game_logger == nullptr)
        init_game_logger();

    return _game_logger;
}