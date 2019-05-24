#include "msg_cereal.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include <fstream>
#include <iostream>

int main() {
    {
        std::ofstream os("msg_test.json");
        cereal::JSONOutputArchive oarchive(os);

        std::shared_ptr<Message> ptr2 = std::make_shared<Unit_move_request2>();
        ptr2->log();
        oarchive(ptr2);
    }

    {
        std::ifstream is("msg_test.json");
        cereal::JSONInputArchive iarchive(is);

        std::shared_ptr<Unit_move_request> ptr2;
        iarchive(ptr2);
        ptr2->log2();
    }

    return 0;
}
