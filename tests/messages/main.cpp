#include "msg_cereal.h"

#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <fstream>
#include <iostream>

int main() {
    {
        std::ofstream os("msg_test.json");
        cereal::JSONOutputArchive oarchive(os);

        auto ptr2                          = std::make_shared<Unit_move_request>();
        ptr2->_hex_ids                     = {23, 3, 45, 6, 7, 10};
        ptr2->_unit_id                     = 102;
        std::shared_ptr<Message> ptr2_base = ptr2;
   //     ptr2_base->log();
        oarchive(ptr2_base);
    }

    {
        std::ifstream is("msg_test.json");
        cereal::JSONInputArchive iarchive(is);

        std::shared_ptr<Message> ptr2;
        iarchive(ptr2);
    //    ptr2->log();
    }

    return 0;
}
