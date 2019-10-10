#include "core/application.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include "core/log.h"

void Application::run() {
    engine_info("Initializing application.");
    initialize();

    engine_info("Initializing ImGui.");
    ImGui::SFML::Init(_window);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    _running = true;
    sf::Clock clock;
    while (_running) {
        sf::Event event;
        while (_window.pollEvent(event)) {
            handle_event(event);

            ImGui::SFML::ProcessEvent(event);
        }
        const auto elapsed_time = clock.restart();
        ImGui::SFML::Update(_window, elapsed_time);
        update(elapsed_time);

        _window.clear();
        render();
        ImGui::SFML::Render(_window);
        _window.display();
        clear_loop();
    }

    _window.close();
    ImGui::SFML::Shutdown();

    engine_info("Quiting application.");
    finalize();
}

void Application::handle_event(const sf::Event& event) {
    const auto& io = ImGui::GetIO();

    switch (event.type) {
        case sf::Event::Closed:
            _running = false;
            break;

        case sf::Event::KeyPressed:
            if (io.WantCaptureKeyboard)
                break;
            key_pressed_event(event.key.code);
            break;

        case sf::Event::KeyReleased:
            if (io.WantCaptureKeyboard)
                break;
            key_released_event(event.key.code);
            break;

        case sf::Event::MouseButtonPressed:
            if (io.WantCaptureMouse)
                break;
            mouse_button_pressed_event(event.mouseButton.button,
                                       _window.mapPixelToCoords(sf::Vector2i(
                                           event.mouseButton.x, event.mouseButton.y)));
            break;

        case sf::Event::MouseButtonReleased:
            if (io.WantCaptureMouse)
                break;
            mouse_button_released_event(event.mouseButton.button,
                                        _window.mapPixelToCoords(sf::Vector2i(
                                            event.mouseButton.x, event.mouseButton.y)));
            break;

        case sf::Event::Resized:
            window_resize_event(event.size.width, event.size.height);
            break;

        case sf::Event::MouseWheelScrolled:
            if (io.WantCaptureMouse)
                break;
            mouse_wheel_scrolled_event(event.mouseWheelScroll.delta);

        case sf::Event::MouseMoved:
            if (io.WantCaptureMouse)
                break;
            mouse_moved_event(_window.mapPixelToCoords(
                sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));

        default:
            break;
    }
}
