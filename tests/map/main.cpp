#include <iostream>
#include <string>
#include <memory>

#include "map/concrete_hex.h"
#include "map/concrete_passage.h"
#include "map/map.h"

#include <SFML/Window.hpp>



int main() {
    /*pugi::xml_document doc;

    std::unique_ptr<Map_site> hex = std::make_unique<Forest>(2);
    hex->serialize(doc);

    doc.save_file("test.xml");*/


    sf::RenderWindow window(sf::VideoMode(800, 600), "Theater of combat");
    window.clear();

 //   auto map = Map::create_test_map(30);
   // map.save("resources/maps/map.xml");
    Map map;
    map.load("resources/maps/map.xml", 30);

    map.draw(window);

    window.display();
    std::cin.get();
    window.close();
}