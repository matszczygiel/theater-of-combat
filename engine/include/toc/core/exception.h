#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>

class EngineException : public std::runtime_error {
   public:
    explicit EngineException(const std::string& msg);
};

class ClientException : public std::runtime_error {
   public:
    explicit ClientException(const std::string& msg);
};

#endif