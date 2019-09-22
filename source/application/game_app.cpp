#include "game_app.h"

#include <sstream>

#include <imgui.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/memory.hpp>
#include <sol/sol.hpp>

#include "core/log.h"
#include "core/lua_vm.h"
#include "gui/dock_space.h"
#include "gui/log_window.h"
#include "gui/network_prompt.h"
#include "lua/lua_map.h"
#include "lua/lua_units.h"
#include "unit/unit_components.h"
#include "unit/unit_manager.h"

Game::Game() {
    auto rot_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "tmp/logs/log.txt", 1048576 * 5, 2);
    rot_sink->set_pattern("%^[%c] (thread %t) %n: [%l] %v%$");
    rot_sink->set_level(spdlog::level::trace);
    logger::get_distributing_sink()->add_sink(rot_sink);

    _res_manager.register_resource_type<Map>("maps", "map");
    _res_manager.register_resource_type<UnitManager>("units", "umg");
}

void Game::initialize() {
    engine_trace("Creating a window.");
    _window.create(sf::VideoMode(1200, 800), "Theater of combat");
    _window.setFramerateLimit(60);

    app_info("Loading font.");
    _gfx_state.map.font.loadFromFile("resources/fonts/OpenSans-Regular.ttf");
    auto& map = *_state.scenario.map;
    map       = Map::create_test_map();

    auto& lua = lua::get_state();
    map::lua_push_functions();
    lua["game_map"] = std::ref(map);
    lua["save_map"] = [&](std::string name) { _res_manager.save(map, name); };
    lua["load_map"] = [&](std::string name) {
        map = _res_manager.load<Map>(name);
    };

    units::lua_push_functions();
    auto& units       = *_state.scenario.units;
    lua["game_units"] = std::ref(units);
    lua["save_units"] = [&](std::string name) {
        _res_manager.save(units, name);
    };
    lua["load_units"] = [&](std::string name) {
        units = _res_manager.load<UnitManager>(name);
    };

    auto unit_id_0  = units.create(UnitType::mechanized, "test unit 0", true);
    auto unit_id_1  = units.create(UnitType::mechanized, "test unit 1", true);
    auto cmp_0      = units.get_component<MovementComponent>(unit_id_0);
    cmp_0->position = HexCoordinate(-1, 1);

    auto cmp_1      = units.get_component<MovementComponent>(unit_id_1);
    cmp_1->position = HexCoordinate(-3, 0);

    _state.scenario.teams["team 0"] = {unit_id_0};
    _state.scenario.teams["team 1"] = {unit_id_1};

    _state.players = {Player("player 0", {"team 0"}),
                      Player("player 1", {"team 1"})};

    lua["switch_action_provider"] = [&]() {
        switch (_action_provider) {
            case ActionProvider::remote:
                _action_provider = ActionProvider::local;
                app_info("_action_provider switched to local.");
                return;
            case ActionProvider::local:
                _action_provider = ActionProvider::remote;
                app_info("_action_provider switched to remote.");
                return;
        }
    };

    lua["undo_action"] = [&]() {
        _state.push_action(std::make_unique<UndoPreviousAction>());
    };

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

    if (_action_provider == ActionProvider::local) {
        for (auto& a : _pending_actions) {
            auto header = static_cast<sf::Int8>(PacketHeader::action);
            std::ostringstream ss{std::ios::out | std::ios::binary};
            {
                cereal::PortableBinaryOutputArchive ar(ss);
                ar(a);
            }
            sf::Packet p;
            p << header << ss.str();
            app_debug("Sending packet...");
            std::visit([&](auto&& net) { net.send(p); }, _network);
            app_debug("Done");
            _state.push_action(std::move(a));
        }
        _pending_actions.clear();

    } else if (_action_provider == ActionProvider::remote) {
        _pending_actions.clear();

        sf::Int8 header = 0;
        std::string str;
        sf::Packet p;
        std::visit([&](auto&& net) { net.receive(p); }, _network);
        p >> header >> str;
        if (header == 1) {
            app_debug("Received packet");
            std::unique_ptr<Action> a{nullptr};
            std::istringstream ss{str, std::ios::in | std::ios::binary};
            {
                cereal::PortableBinaryInputArchive ar(ss);
                ar(a);
            }

            _state.push_action(std::move(a));
        }
    }

    show_dock_space_window(nullptr);

    static bool show_demo_window{true};
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    _log.show_window(nullptr);
    _console.show(nullptr);

    show_network_prompt(_network, "Network status", nullptr);

    _gfx_state.update();

    // engine_trace("Updating");
}

void Game::render() { _gfx_state.draw(_window); }

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
            _gfx_state.layout->size.y /= map_tilt_speed;
            break;
        case sf::Keyboard::S:
            _gfx_state.layout->size.y *= map_tilt_speed;
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
            const auto coord = world_point_to_hex(position, *_gfx_state.layout);
            if (!_moving_system->is_moving()) {
                _moving_system->init_movement(coord);
            } else {
                auto action = _moving_system->move_target(coord);
                _pending_actions.push_back(std::move(action));
            }
        } break;

        default:
            break;
    }
}

void Game::mouse_button_released_event(const sf::Mouse::Button&,
                                       const sf::Vector2f&) {}

void Game::mouse_wheel_scrolled_event(const float& delta) {
    auto view                   = _window.getView();
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
    _gfx_state.highlighted_hexes.clear();
    const auto coord = world_point_to_hex(position, *_gfx_state.layout);

    if (_moving_system->is_moving()) {
        const auto path = _moving_system->path_preview(coord);
        for (const auto [hex_coord, shape] : _gfx_state.map.hexes) {
            if (std::any_of(path.cbegin(), path.cend(),
                            [hex_coord = hex_coord](const auto& x) {
                                return x == hex_coord;
                            })) {
                _gfx_state.highlighted_hexes.push_back(shape.highlighting_shape());
            }
        }
    } else {
        _gfx_state.highlighted_hexes.push_back(
            std::find_if(_gfx_state.map.hexes.cbegin(), _gfx_state.map.hexes.cend(),
                         [&](const auto& hex) { return hex.first == coord; })
                ->second.highlighting_shape());
    }
}