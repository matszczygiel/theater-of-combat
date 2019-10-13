#include "core/exception.h"

#include <string>

EngineException::EngineException(const std::string& msg)
    : std::runtime_error("Paused on EngineException. Message: " + msg) {}

ClientException::ClientException(const std::string& msg)
    : std::runtime_error("Paused on ClientException. Message: " + msg) {}