#include "core/application.h"

#include <exception>
#include <iostream>

#include <imgui-SFML.h>
#include <imgui.h>

#include "core/log.h"

Application::Application() noexcept {
    std::set_terminate([]() {
        auto eptr = std::current_exception();

        std::cerr << "Terminating program due to unhandled exception.\n";
        engine_critical("Terminating program due to unhandled exception.");

        try {
            if (eptr)
                std::rethrow_exception(eptr);

        } catch (const EngineException& e) {
            std::cerr << "Uncaught EngineException: " << e.what() << '\n';
            engine_critical("Uncaught EngineException: {}", e.what());

        } catch (const ClientException& e) {
            std::cerr << "Uncaught ClientException: " << e.what() << '\n';
            engine_critical("Uncaught ClientException: {}", e.what());

        } catch (const std::exception& e) {
            std::cerr << "Uncaught std::exception: " << e.what() << '\n';
            engine_critical("Uncaught std::exception: {}", e.what());

        } catch (...) {
            std::cerr << "Uncaught exception of unknown type\n";
            engine_critical("Uncaught exception of unknown type");
        }

        std::cerr << "Aborting...\n";
        engine_critical("Aborting...");

        std::abort();
    });
}

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
    switch (event.type) {
        case sf::Event::Closed:
            _running = false;
            break;

        case sf::Event::KeyPressed:
            if (imgui_want_keyboard())
                break;
            key_pressed_event(event.key.code);
            break;

        case sf::Event::KeyReleased:
            if (imgui_want_keyboard())
                break;
            key_released_event(event.key.code);
            break;

        case sf::Event::MouseButtonPressed:
            if (imgui_want_mouse())
                break;
            mouse_button_pressed_event(event.mouseButton.button,
                                       _window.mapPixelToCoords(sf::Vector2i(
                                           event.mouseButton.x, event.mouseButton.y)));
            break;

        case sf::Event::MouseButtonReleased:
            if (imgui_want_mouse())
                break;
            mouse_button_released_event(event.mouseButton.button,
                                        _window.mapPixelToCoords(sf::Vector2i(
                                            event.mouseButton.x, event.mouseButton.y)));
            break;

        case sf::Event::Resized:
            window_resize_event(event.size.width, event.size.height);
            break;

        case sf::Event::MouseWheelScrolled:
            if (imgui_want_mouse())
                break;
            mouse_wheel_scrolled_event(event.mouseWheelScroll.delta);

        case sf::Event::MouseMoved:
            if (imgui_want_mouse())
                break;
            mouse_moved_event(_window.mapPixelToCoords(
                sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));

        default:
            break;
    }
}

bool Application::imgui_want_mouse() {
    const auto& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool Application::imgui_want_keyboard() {
    const auto& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}
