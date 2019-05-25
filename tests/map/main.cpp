#include <iostream>
#include <string>

#include "map/concrete_hex.h"

int main() {
    /*pugi::xml_document doc;

    std::unique_ptr<Map_site> hex = std::make_unique<Forest>(2);
    hex->serialize(doc);

    doc.save_file("test.xml");*/

    pugi::xml_document doc;
    auto parsing_result = doc.load_file("test.xml");
    auto node = doc.child(Map_site::object_identifier);
    auto hex = Map_site::unserialize(node);
    std::cout << hex->get_number() << " " << hex->get_name() << '\n';

}