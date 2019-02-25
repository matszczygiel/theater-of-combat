#include "application.h"


#include "log.h"

void Application::run() {
    
    ENGINE_INFO("Initializing application.");
    initialize();

    sf::Clock clock;
    while (_running) {

        sf::Event event;
        while(_window.pollEvent(event)){
            handle_event(event);
        }

        const auto elapsed_time = clock.restart();
        update(elapsed_time);
        render();
    }

    _window.close();
    
    ENGINE_INFO("Quiting application.");
    finalize();
}