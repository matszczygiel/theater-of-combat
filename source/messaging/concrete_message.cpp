#pragma once

#include <regex>
#include <sstream>

#include "concrete_message.h"

const Message::id_type Unit_moved_msg::name = "Unit_moved_msg";

const Message::id_type& Unit_moved_msg::get_name() const {
    return name;
}

Unit_moved_msg::Unit_moved_msg(const int& hexno, const std::string& unit_name, const std::string& nation)
    : _hexno(hexno), _unit_name(unit_name), _nation(nation) {}

Unit_moved_msg::Unit_moved_msg(const std::string& stream) {
    {
        std::regex rg("h\\s+\\d+\\s", std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(stream, match, rg)) {
            std::stringstream ss(match.str());
            ss.get();
            ss >> _hexno;
        }
    }
{
        std::regex rg("u\\s+\\S+\\s", std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(stream, match, rg)) {
            std::stringstream ss(match.str());
            ss.get();
            ss >> _unit_name;
        }
    }
{
        std::regex rg("n\\s+\\S+\\s", std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(stream, match, rg)) {
            std::stringstream ss(match.str());
            ss.get();
            ss >> _nation;
        }
    }
}