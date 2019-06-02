#include <fstream>
#include <memory>
#include <string>

#include "map/concrete_hex.h"
#include "map/concrete_passage.h"
#include "map/map.h"

#include <SFML/Window.hpp>
#include <cereal/archives/xml.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Theater of combat");
    window.clear();
    /*
    auto map = Map::create_test_map(30);

{
    std::ofstream os("resources/maps/cereal_map.xml");
    cereal::XMLOutputArchive oarchive(os);

    oarchive(map);
}*/

    Map map;

    {
        std::ifstream is("resources/maps/cereal_map.xml");
        cereal::XMLInputArchive iarchive(is);
        iarchive(map);
    }

    map.recompute_geometry(30);

    map.draw(window);
    window.display();
    std::cin.get();
    window.close();
}