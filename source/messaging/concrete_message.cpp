#include <regex>
#include <sstream>

#include "concrete_message.h"

Request_unit_movement::Request_unit_movement(const int& hexno, const std::string& unit_name, const std::string& nation)
    : _hexno(hexno), _unit_name(unit_name), _nation(nation) {}

Request_unit_movement::Request_unit_movement(const std::string& stream) {
    {
        std::regex rg("h\\s+\\d+", std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(stream, match, rg)) {
            std::stringstream ss(match.str());
            ss.get();
            ss >> _hexno;
        }
    }
    {
        std::regex rg("u\\s+\\S+", std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(stream, match, rg)) {
            std::stringstream ss(match.str());
            ss.get();
            ss >> _unit_name;
        }
    }
    {
        std::regex rg("n\\s+\\S+", std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(stream, match, rg)) {
            std::stringstream ss(match.str());
            ss.get();
            ss >> _nation;
        }
    }
}

std::string Request_unit_movement::to_string() const {
    return "h " + std::to_string(_hexno) + " u " + _unit_name + " n " + _nation + "\n";
}