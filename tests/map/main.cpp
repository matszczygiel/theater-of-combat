#include <iostream>
#include <string>

#include "map/concrete_hex.h"
#include "map/map.h"

#include <SFML/Window.hpp>



int main() {
    /*pugi::xml_document doc;

    std::unique_ptr<Map_site> hex = std::make_unique<Forest>(2);
    hex->serialize(doc);

    doc.save_file("test.xml");*/

    pugi::xml_document doc;
    auto parsing_result = doc.load_file("test.xml");
    auto node           = doc.child(Map_site::object_identifier);
    auto hex            = Map_site::unserialize(node);
    std::cout << hex->get_number() << " " << hex->get_name() << '\n';

    sf::RenderWindow window(sf::VideoMode(800, 600), "Theater of combat");
    window.clear();

    auto map = Map::create_test_map(30);
    map.draw(window);

    window.display();
    std::cin.get();
    window.close();
}