#include "application.h"

#include "log.h"

void Application::run() {
    ENGINE_INFO("Initializing application.");

    initialize();
    _gui.setTarget(_window);

    sf::Clock clock;
    while (_running) {
        sf::Event event;
        while (_window.pollEvent(event)) {
            handle_event(event);
        }

        const auto elapsed_time = clock.restart();
        update(elapsed_time);
        _window.clear();
        render();
        _gui.draw();
        _window.display();
    }

    _window.close();

    ENGINE_INFO("Quiting application.");
    finalize();
}

void Application::handle_event(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed:
            _running = false;
            break;

        case sf::Event::KeyPressed:
            key_pressed_event(event.key.code);
            break;

        case sf::Event::KeyReleased:
            key_released_event(event.key.code);
            break;

        case sf::Event::MouseButtonPressed:
            mouse_button_pressed_event(event.mouseButton.button,
                                       _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
            break;

        case sf::Event::MouseButtonReleased:
            mouse_button_released_event(event.mouseButton.button,
                                        _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
            break;

        case sf::Event::Resized:
            window_resize_event(event.size.width, event.size.height);
            break;

        case sf::Event::MouseWheelScrolled:
            mouse_wheel_scrolled_event(event.mouseWheelScroll.delta);

        default:
            break;
    }
}