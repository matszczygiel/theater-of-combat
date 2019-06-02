#include "msg_cereal.h"

#include <iostream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/polymorphic.hpp>

void Unit_move_request::log() const {
    std::cout << "log \n";
}

void Unit_move_request2::log2() const {
    std::cout << "log2 \n";
}

CEREAL_REGISTER_TYPE(Unit_move_request2)
