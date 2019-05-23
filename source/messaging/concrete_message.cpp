#include <regex>
#include <sstream>

#include "concrete_message.h"

Request_unit_movement::Request_unit_movement(const int& hexno, const std::string& unit_name, const std::string& nation)
    : _hexno(hexno), _unit_name(unit_name), _nation(nation) {}

Request_unit_movement::Request_unit_movement(const std::string& stream)
    : _hexno(Message::get_entry_from_stream<int>(stream, "h")),
      _unit_name(Message::get_entry_from_stream<std::string>(stream, "u")),
      _nation(Message::get_entry_from_stream<std::string>(stream, "n")) {}

std::string Request_unit_movement::to_string() const {
    return get_name() + " h " + std::to_string(_hexno) + " u " + _unit_name + " n " + _nation + "\n";
}