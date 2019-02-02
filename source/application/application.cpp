#include "application.h"


#include "log.h"

void Application::run() {
    
    ENGINE_INFO("Initializing application.");
    initialize();

    while (_running) {

        sf::Event event;
        while(_window.pollEvent(event)){
            handle_event(event);
        }

        update();
        render();
    }

    _window.close();
    
    ENGINE_INFO("Quiting application.");
    finalize();
}