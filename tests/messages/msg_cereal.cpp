#include "msg_cereal.h"

#include <iostream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/polymorphic.hpp>
/*oid Unit_move_request::log() const {
    std::cout << "hhahah\n";
}
*/
CEREAL_REGISTER_TYPE(Unit_move_request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, Unit_move_request)