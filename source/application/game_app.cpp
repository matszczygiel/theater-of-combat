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
#include "lua/lua_gameplay.h"
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
    _gfx_state.font.loadFromFile("resources/fonts/OpenSans-Regular.ttf");

    auto& map = _state.scenario->map;
    auto& lua = lua::get_state();
    map::lua_push_functions();
    lua["game_map"]      = std::ref(map);
    lua["save_map_json"] = [&](std::string name) {
        _res_manager.save_json(map, name);
    };
    lua["load_map_json"] = [&](std::string name) {
        map = _res_manager.load_json<Map>(name);
    };

    units::lua_push_functions();
    auto& units            = _state.scenario->units;
    lua["game_units"]      = std::ref(units);
    lua["save_units_json"] = [&](std::string name) {
        _res_manager.save_json(units, name);
    };
    lua["load_units_json"] = [&](std::string name) {
        units = _res_manager.load_json<UnitManager>(name);
    };

    lua["load_units_test"] = [&]() {
        units = UnitManager::create_test_manager();
    };

    lua["undo_action"] = [&]() {
        _pending_actions.push_back(std::make_unique<UndoPreviousAction>());
    };

    gameplay::lua_push_functions();
    lua["game_state"]           = std::ref(_state);
    lua["game_scenario"]        = std::ref(*_state.scenario);
    lua["load_scenario_script"] = [&](std::string name) {
        std::ifstream lua_script(_res_manager.resources_path().string() +
                                 "/scenarios/" + name + ".lua");
        if (!lua_script.is_open())
            return false;

        std::string str((std::istreambuf_iterator<char>(lua_script)),
                        std::istreambuf_iterator<char>());

        if (!_state.scenario->load_script(str))
            return false;

        auto units_config = lua["graphics_config"]["units"];
        if (!units_config.valid())
            return false;

        std::string texture_path = units_config["file"];
        texture_path             = "resources/textures/" + texture_path;
        const int size           = units_config["token_size"].get_or(0);

        std::map<Unit::IdType, sf::IntRect> texture_rects;
        for (const auto& [id, _] : _state.scenario->units.units()) {
            auto vec = units_config["specific_tokens"][id];
            if (vec.valid()) {
                const int x = vec["x"].get_or(0);
                const int y = vec["y"].get_or(0);
                texture_rects.emplace(
                    id, sf::IntRect(size * x, size * y, size, size));
            }
        }

        _gfx_state.units.setup(_state.scenario->units, texture_path,
                               texture_rects);

        auto map_config = lua["graphics_config"]["map"];
        if (!map_config.valid())
            return false;

        texture_path        = map_config["tiles_file"];
        texture_path        = "resources/textures/" + texture_path;
        const int tile_size = map_config["tile_size"].get_or(0);

        sol::table hex_tiles = map_config["hex_tiles"];
        std::map<HexType, sf::IntRect> tile_rects;
        for (const auto& entry : hex_tiles) {
            const auto key = static_cast<HexType>(entry.first.as<int>());
            const int x    = entry.second.as<sol::table>()["x"].get_or(0);
            const int y    = entry.second.as<sol::table>()["y"].get_or(0);
            tile_rects.emplace(key, sf::IntRect(tile_size * x, tile_size * y,
                                                tile_size, tile_size));
        }
        _gfx_state.map.setup(_state.scenario->map, texture_path, tile_rects);

        return true;
    };

    lua["local_player_name"]       = std::ref(_local_player_name);
    lua["parse_local_player_name"] = [&]() {
        return _state.set_local_player(_local_player_name);
    };
    lua["set_local_player_index"] = [&](int i) { _state.set_local_player(i); };

    lua.script(R"(
    load_scenario_script('test0')
    set_local_player_index(0)
    game_state:start()
    )");
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

    if (_state.is_local_player_now()) {
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
        }

    } else {
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

            _pending_actions.push_back(std::move(a));
        }
    }

    for (auto& a : _pending_actions) {
        debug_info.debug_action(a);
        _state.push_action(std::move(a));
    }
    _pending_actions.clear();

    show_dock_space_window(nullptr);

    static bool show_demo_window{true};
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    _log.show_window(nullptr);
    _console.show(nullptr);

    show_network_prompt(_network, "Network status", nullptr);

    _gfx_state.update();

    if(_state.phase == GamePhase::battles){
        _state.next_phase();
        _fight_system.make_fight_stack(_state.current_player_index());
        auto actions = _fight_system.compute_fight_result();
        std::move(std::begin(actions), std::end(actions), std::back_inserter(_pending_actions));
    }

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
            if (_state.is_local_player_now() &&
                _state.phase == GamePhase::movement) {
                if (!_moving_system.is_moving()) {
                    _moving_system.init_movement(
                        coord,
                        _state.scenario
                            ->player_teams[_state.current_player_index()],
                        _state.scenario
                            ->player_teams[_state.opposite_player_index()]);
                } else {
                    auto action = _moving_system.move_target(coord);
                    _pending_actions.push_back(std::move(action));
                }
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

    if (_moving_system.is_moving()) {
        const auto path = _moving_system.path_preview(coord);
        for (const auto [hex_coord, shape] : _gfx_state.map.hexes) {
            if (std::any_of(path.cbegin(), path.cend(),
                            [hex_coord = hex_coord](const auto& x) {
                                return x == hex_coord;
                            })) {
                _gfx_state.highlighted_hexes.push_back(
                    shape.highlighting_shape());
            }
        }
    } else {
        if (!_gfx_state.map.hexes.empty()) {
            if (auto it = std::find_if(
                    _gfx_state.map.hexes.cbegin(), _gfx_state.map.hexes.cend(),
                    [&](const auto& hex) { return hex.first == coord; });
                it != _gfx_state.map.hexes.cend()) {
                _gfx_state.highlighted_hexes.push_back(
                    it->second.highlighting_shape());
            }
        }
    }
}