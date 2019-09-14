#include "log.h"

#include <spdlog/spdlog.h>

namespace logger {
std::shared_ptr<spdlog::sinks::dist_sink_mt>& get_distributing_sink() {
    static auto sink = std::make_shared<spdlog::sinks::dist_sink_mt>();
    return sink;
}

std::unique_ptr<spdlog::logger>& get_app_logger() {
    static auto logger = []() {
        auto logger =
            std::make_unique<spdlog::logger>("CLIENT", get_distributing_sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::critical);
        return logger;
    }();
    return logger;
}

std::unique_ptr<spdlog::logger>& get_engine_logger() {
    static auto logger = []() {
        auto logger =
            std::make_unique<spdlog::logger>("ENGINE", get_distributing_sink());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::critical);
        return logger;
    }();
    return logger;
}
}  // namespace logger
