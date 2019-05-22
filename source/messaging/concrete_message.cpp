#pragma once

#include <sstream>
#include <regex>

#include "concrete_message.h"

const Message::id_type Unit_moved_msg::name = "Unit_moved_msg";

const Message::id_type& Unit_moved_msg::get_name() const {
    return name;
}

Unit_moved_msg::Unit_moved_msg(const int& hexno, const std::string& unit_name, const std::string& nation) 
: _hexno(hexno), _unit_name(unit_name), _nation(nation) {}

Unit_moved_msg::Unit_moved_msg(const std::string& stream) {
    std::stringstream ss(stream);
    std::string token;
    int hexno;
    std::string 
    while(ss >> token){
        if(token == "h") {
            ss >> hexno;
        }
        else if (token == "u") {
            ss >>
        }

    }

        std::regex rg("h\s+*\s");
        std::smatch match;
        if(std::regex_search(stream, match, rg))


    std::string line;
    while (std::getline(input_file, line)) {
        if (line.empty())
            continue;

        std::regex reg("\\s+");

        std::sregex_token_iterator beg(line.begin(), line.end(), reg, -1);
        std::sregex_token_iterator end;

        std::string key = *beg;
        std::vector<std::string> vec(++beg, end);
        keys.insert(std::make_pair(key, vec));
}
}