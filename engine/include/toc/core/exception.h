#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>

class EngineException : public std::runtime_error {
   public:
    EngineException(const std::string& msg);
};

class ClientException : public std::runtime_error {
   public:
    ClientException(const std::string& msg);
};

#endif