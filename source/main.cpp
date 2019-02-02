#include <iostream>

#include <SFML/Graphics.hpp>

#include "application/game_app.h"
#include "log.h"

int main() {
    ENGINE_INFO("Starting the engine.");
    Game app;
    app.run();

    return EXIT_SUCCESS;
}