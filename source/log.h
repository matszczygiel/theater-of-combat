#pragma once

#include <memory>

#include <spdlog/logger.h>

class Log {
   public:
    Log() = delete;

    static std::shared_ptr<spdlog::logger> get_engine_logger();
    static std::shared_ptr<spdlog::logger> get_game_logger();

   private:
    static void init_game_logger();
    static void init_engine_logger();

    static std::shared_ptr<spdlog::logger> _engine_logger;
    static std::shared_ptr<spdlog::logger> _game_logger;
};

#define ENGINE_TRACE(...) ::Log::get_engine_logger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...) ::Log::get_engine_logger()->info(__VA_ARGS__)
#define ENGINE_WARN(...) ::Log::get_engine_logger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...) ::Log::get_engine_logger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...) ::Log::get_engine_logger()->critical(__VA_ARGS__)

#define GAME_TRACE(...) ::Log::get_game_logger()->trace(__VA_ARGS__)
#define GAME_INFO(...) ::Log::get_game_logger()->info(__VA_ARGS__)
#define GAME_WARN(...) ::Log::get_game_logger()->warn(__VA_ARGS__)
#define GAME_ERROR(...) ::Log::get_game_logger()->error(__VA_ARGS__)
#define GAME_CRITICAL(...) ::Log::get_game_logger()->critical(__VA_ARGS__)
