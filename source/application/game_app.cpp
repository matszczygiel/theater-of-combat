#include "game_app.h"

#include <imgui.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sol/sol.hpp>

#include "core/log.h"
#include "core/lua_vm.h"
#include "gui/dock_space.h"
#include "gui/log_window.h"
#include "lua/lua_map.h"
#include "lua/lua_units.h"
#include "unit/unit_components.h"

Game::Game() {
    auto rot_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "tmp/logs/log.txt", 1048576 * 5, 2);
    rot_sink->set_pattern("%^[%c] (thread %t) %n: [%l] %v%$");
    rot_sink->set_level(spdlog::level::trace);
    logger::get_distributing_sink()->add_sink(rot_sink);

    _res_manager.register_resource_type<Map>("maps", "map");
}

void Game::initialize() {
    engine_trace("Creating a window.");
    _window.create(sf::VideoMode(1200, 800), "Theater of combat");
    _window.setFramerateLimit(60);

    app_info("Loading font.");
    _map_gfx.font.loadFromFile("resources/fonts/OpenSans-Regular.ttf");
    _map_gfx.layout->size = sf::Vector2f{50.f, 50.f};

    *_map = Map::create_test_map();
    _map_gfx.update(*_map);

    auto& lua = lua::get_state();
    map::lua_push_functions();
    lua["game_map"] = &_map;
    lua["save_map"] = [&](std::string name) { _res_manager.save(_map, name); };
    lua["load_map"] = [&](std::string name) { *_map = _res_manager.load<Map>(name); };

    units::lua_push_functions();
    lua["game_units"] = &_units;

    auto unit_id = _units->create(UnitType::mechanized, "test unit", true);
    auto cmp = _units->get_component<MovementComponent>(unit_id);
    cmp->position = HexCoordinate(-1, 1);

    _unit_gfx.update(*_units);
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

    show_dock_space_window(nullptr);

    static bool show_demo_window{true};
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    _log.show_window(nullptr);
    _console.show(nullptr);

    _map_gfx.update(*_map);
    _unit_gfx.update(*_units);


    // engine_trace("Updating");
}

void Game::render() {
    _map_gfx.draw_hexes(_window);
    _map_gfx.draw_rivers(_window);
    _map_gfx.draw_outlines(_window);

    for (const auto& shape : _highlighted_hexes) {
        _window.draw(shape);
    }

    _map_gfx.draw_coords(_window);

    _unit_gfx.draw_tokens(_window);
}

void Game::clear_loop() {}

void Game::finalize() {}

void Game::key_pressed_event(const sf::Keyboard::Key& key) {
    constexpr float map_tilt_speed = 1.05;
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
            _map_gfx.layout->size.y /= map_tilt_speed;
            break;
        case sf::Keyboard::S:
            _map_gfx.layout->size.y *= map_tilt_speed;
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
        case sf::Mouse::Left: {
            const auto coord = world_point_to_hex(position, *_map_gfx.layout);
            if(!_moving_system->is_moving()) {
                _moving_system->init_movement(coord);
            }
        }
            break;

        default:
            break;
    }
}

void Game::mouse_button_released_event(const sf::Mouse::Button&,
                                       const sf::Vector2f& ) {}

void Game::mouse_wheel_scrolled_event(const float& delta) {
    auto view = _window.getView();
    constexpr float zoom_factor = 1.05;
    if (delta > 0)
        view.zoom(1.0 / zoom_factor);
    else if (delta < 0)
        view.zoom(zoom_factor);

    _window.setView(view);
}

void Game::window_resize_event(const unsigned& width, const unsigned& height) {
    auto view = _window.getView();
    view.setSize(static_cast<float>(width), static_cast<float>(height));
    _window.setView(view);
}

void Game::mouse_moved_event(const sf::Vector2f& position) {
    _highlighted_hexes.clear();
    const auto coord = world_point_to_hex(position, *_map_gfx.layout);
    _highlighted_hexes.push_back(
        std::find_if(_map_gfx.hexes.begin(), _map_gfx.hexes.end(),
                     [&](const auto& hex) { return hex.first == coord; })
            ->second.highlighting_shape());

    if(_moving_system->is_moving()) {
        const auto path = _moving_system->path_preview(coord);
        for(const auto& [coord, shape] : _map_gfx.hexes()){
            if(std::find(path.begin(), path.end(), coord) != path.end()) {
                _highlighted_hexes.push_back(shape.highlighting_shape());
            }
        }
        
    }
}