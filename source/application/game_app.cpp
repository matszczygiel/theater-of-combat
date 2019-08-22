#include "game_app.h"

#include <imgui.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/rotating_file_sink.h"

#include "gui/log_window.h"
#include "log.h"

Game::Game() {
    auto rot_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "tmp/logs/log.txt", 1048576 * 5, 2);
    rot_sink->set_pattern("%^[%T] (thread %t) %n [%l]: %v%$");
    rot_sink->set_level(spdlog::level::trace);
    logger::get_distributing_sink()->add_sink(rot_sink);
}

void Game::initialize() {
    engine_trace("Creating a window.");
    _window.create(sf::VideoMode(800, 600), "Theater of combat");
    _window.setFramerateLimit(60);

    game_info("Loading font");
    _map_gfx.font.loadFromFile("resources/fonts/OpenSans-Regular.ttf");
    _map_gfx.layout->size = sf::Vector2f{50.f, 50.f};

    _map = Map::create_test_map();
    _map_gfx.update(_map);
}

void Game::update(const sf::Time& elapsed_time) {
    auto view = _window.getView();
    sf::Vector2f moving_view{0.f, 0.f};
    if (_moving_view_down)
        moving_view += sf::Vector2f{0.f, 1.f};
    if (_moving_view_up)
        moving_view += sf::Vector2f{0.f, -1.f};
    if (_moving_view_right)
        moving_view += sf::Vector2f{1.f, 0.f};
    if (_moving_view_left)
        moving_view += sf::Vector2f{-1.f, 0.f};

    moving_view *= elapsed_time.asMilliseconds() * _view_moving_speed;

    view.move(moving_view);
    _window.setView(view);

    static bool show_demo_window{true};
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    show_log_window(nullptr);

    _map_gfx.update(_map);

    engine_trace("updating");
}

void Game::render() {
    _map_gfx.draw_hexes(_window);
    _map_gfx.draw_rivers(_window);
    _map_gfx.draw_outlines(_window);

    for (const auto& shape : _highlighted_hexes) {
        _window.draw(shape);
    }

    _map_gfx.draw_coords(_window);
}

void Game::clear_loop() {}

void Game::finalize() {}

void Game::key_pressed_event(const sf::Keyboard::Key& key) {
    switch (key) {
        case sf::Keyboard::Escape:
            _running = false;
            break;
        case sf::Keyboard::Up:
            _moving_view_up = true;
            break;
        case sf::Keyboard::Down:
            _moving_view_down = true;
            break;
        case sf::Keyboard::Right:
            _moving_view_right = true;
            break;
        case sf::Keyboard::Left:
            _moving_view_left = true;
            break;
        case sf::Keyboard::W:
            _map_gfx.layout->size.y *= 0.95;
            break;
        case sf::Keyboard::S:
            _map_gfx.layout->size.y *= 1.05;
            break;
        default:
            break;
    }
}

void Game::key_released_event(const sf::Keyboard::Key& key) {
    switch (key) {
        case sf::Keyboard::Up:
            _moving_view_up = false;
            break;

        case sf::Keyboard::Down:
            _moving_view_down = false;
            break;

        case sf::Keyboard::Right:
            _moving_view_right = false;
            break;

        case sf::Keyboard::Left:
            _moving_view_left = false;
            break;

        default:
            break;
    }
}

void Game::mouse_button_pressed_event(const sf::Mouse::Button& button,
                                      const sf::Vector2f& position) {
    switch (button) {
        case sf::Mouse::Left:
            break;

        default:
            break;
    }
}

void Game::mouse_button_released_event(const sf::Mouse::Button& button,
                                       const sf::Vector2f& position) {}

void Game::mouse_wheel_scrolled_event(const float& delta) {
    auto view = _window.getView();
    if (delta > 0)
        view.zoom(0.95);
    else if (delta < 0)
        view.zoom(1.05);

    _window.setView(view);
}

void Game::window_resize_event(const unsigned& width, const unsigned& height) {
    auto view = _window.getView();
    view.setSize(width, height);
    _window.setView(view);
}

void Game::mouse_moved_event(const sf::Vector2f& position) {
    _highlighted_hexes.clear();
    const auto coord = world_point_to_hex(position, *_map_gfx.layout);
    _highlighted_hexes.push_back(
        std::find_if(_map_gfx.hexes.begin(), _map_gfx.hexes.end(),
                     [&](const auto& hex) { return hex.first == coord; })
            ->second.highlighting_shape());
}