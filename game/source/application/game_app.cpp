#include "game_app.h"

#include <sstream>

#include <imgui.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/memory.hpp>
#include <sol/sol.hpp>

#include "toc/core/log.h"
#include "toc/core/lua_vm.h"
#include "toc/gameplay/actions.h"
#include "toc/gameplay/lua_gameplay.h"
#include "toc/gui/dock_space.h"
#include "toc/gui/log_window.h"
#include "toc/map/lua_map.h"
#include "toc/unit/lua_units.h"
#include "toc/unit/unit_components.h"
#include "toc/unit/unit_manager.h"

#include "tsys/kircholm.h"

#include "gui/menu_bar.h"
#include "gui/network_prompt.h"

Game::Game() {
    auto rot_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "tmp/logs/log.txt", 1048576 * 5, 2);
    rot_sink->set_pattern("%^[%c] (thread %t) %n: [%l] %v%$");
    rot_sink->set_level(spdlog::level::trace);
    logger::get_distributing_sink()->add_sink(rot_sink);

    _res_loader->register_resource_type<Map>("maps", "map");
    _res_loader->register_resource_type<UnitManager>("units", "umg");
    _res_loader->register_resource_type<sf::Font>("fonts", "ttf");
    _res_loader->register_resource_type<sf::Texture>("textures", "png");
    _res_loader->register_resource_type<Scenario>("scenarios", "scn");

    _system = std::make_unique<kirch::SystemKircholm>();
    _debug  = _system->create_debug_info();
}

void Game::initialize() {
    engine_trace("Creating a window.");
    _window.create(sf::VideoMode(1200, 800), "Theater of combat");
    _window.setFramerateLimit(60);

    app_info("Loading font.");
    _system->gfx.font =
        _res_loader->load<decltype(_system->gfx.font)>("OpenSans-Regular");

    auto& map = _system->scenario->map;
    auto& lua = lua::get_state();
    map::lua_push_functions(lua);
    lua["get_game_map"]  = [&map]() -> Map& { return map; };
    lua["save_map_json"] = [&](std::string name) { _res_loader->save_json(map, name); };
    lua["load_map_json"] = [&](std::string name) {
        map = _res_loader->load_json<Map>(name);
    };
    lua["set_game_map"] = [&map](const Map& m) { map = m; };

    units::lua_push_functions(lua);
    auto& units            = _system->scenario->units;
    lua["game_units"]      = std::ref(units);
    lua["save_units_json"] = [&](std::string name) {
        _res_loader->save_json(units, name);
    };
    lua["load_units_json"] = [&](std::string name) {
        units = _res_loader->load_json<UnitManager>(name);
    };

    lua["load_units_test"] = [&]() { units = UnitManager::create_test_manager(); };

    lua["undo_action"] = [&]() {
        _system->push_action(std::make_unique<UndoPreviousAction>());
    };

    gameplay::lua_push_functions(lua);
    lua["game_scenario"]     = std::ref(*_system->scenario);
    lua["system"]            = std::ref(_system);
    lua["next_phase_action"] = [&]() {
        _system->push_action(std::make_unique<NextPhaseAction>());
    };

    _system->prepare_lua(lua);

    lua["load_scenario_script"] = [&](std::string name) {
        std::ifstream lua_script(_res_loader->resources_path().string() + "/scenarios/" +
                                 name + ".lua");
        if (!lua_script.is_open())
            return false;

        std::string str((std::istreambuf_iterator<char>(lua_script)),
                        std::istreambuf_iterator<char>());

        if (!_system->scenario->load_script(lua::get_state(), str))
            return false;

        auto units_config = lua["graphics_config"]["units"];
        if (!units_config.valid())
            return false;

        std::string texture_path = units_config["file"];
        texture_path             = "resources/textures/" + texture_path;
        const int size           = units_config["token_size"].get_or(0);

        std::map<Unit::IdType, sf::IntRect> texture_rects;
        for (const auto& [id, _] : _system->scenario->units.units()) {
            auto vec = units_config["specific_tokens"][id];
            if (vec.valid()) {
                const int x = vec["x"].get_or(0);
                const int y = vec["y"].get_or(0);
                texture_rects.emplace(id, sf::IntRect(size * x, size * y, size, size));
            }
        }

        _system->gfx.units.setup(_system->scenario->units, texture_path, texture_rects);

        auto map_config = lua["graphics_config"]["map"];
        if (!map_config.valid())
            return false;

        texture_path        = map_config["tiles_file"];
        texture_path        = "resources/textures/" + texture_path;
        const int tile_size = map_config["tile_size"].get_or(0);

        sol::table hex_tiles = map_config["hex_tiles"];
        std::map<HexSite::HexType, sf::IntRect> tile_rects;
        for (const auto& entry : hex_tiles) {
            const auto key = entry.first.as<HexSite::HexType>();
            const int x    = entry.second.as<sol::table>()["x"].get_or(0);
            const int y    = entry.second.as<sol::table>()["y"].get_or(0);
            tile_rects.emplace(
                key, sf::IntRect(tile_size * x, tile_size * y, tile_size, tile_size));
        }
        _system->gfx.map.setup(_system->scenario->map, texture_path, tile_rects);

        return true;
    };

    lua["local_player_name"]       = std::ref(_local_player_name);
    lua["parse_local_player_name"] = [&]() {
        return _system->set_local_player(_local_player_name);
    };
    lua["set_local_player_index"] = [&](int i) { _system->set_local_player(i); };

    lua.script(R"(
    load_scenario_script('test0')
    game_units:assign_movement_cmp(1, MovementComponent.new(15))
    game_units:assign_movement_cmp(2, MovementComponent.new(10))
    set_local_player_index(0)
    system:start()
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

    if (_system->is_local_player_now()) {
        for (const auto& a : _system->accumulated_actions) {
            auto header = static_cast<sf::Int8>(PacketHeader::action);
            std::ostringstream ss{std::ios::out | std::ios::binary};
            {
                cereal::PortableBinaryOutputArchive ar(ss);
                ar(a);
            }
            sf::Packet p;
            p << header << ss.str();
            app_debug("Sending packet...");
            std::visit([&](auto&& net) { net.send(p); }, _network->net);
            app_debug("Done");
        }

    } else {
        _system->accumulated_actions.clear();
        sf::Int8 header = 0;
        std::string str;
        sf::Packet p;
        std::visit([&](auto&& net) { net.receive(p); }, _network->net);
        p >> header >> str;
        if (header == 1) {
            app_debug("Received packet");
            std::unique_ptr<Action> a{nullptr};
            std::istringstream ss{str, std::ios::in | std::ios::binary};
            {
                cereal::PortableBinaryInputArchive ar(ss);
                ar(a);
            }

            _system->push_action(std::move(a));
        }
    }

    for (const auto& ac : _system->accumulated_actions)
        _debug->log_action(ac);

    _system->update();

    static MenuOptions menu_opts{};
    show_menu_bar(menu_opts);
    if (menu_opts.show_dock_space)
        show_dock_space_window(&menu_opts.show_dock_space);
    if (menu_opts.show_console)
        _console.show(&menu_opts.show_console);
    if (menu_opts.show_log_console)
        _log.show_window(&menu_opts.show_log_console);
    if (menu_opts.show_imgui_demo)
        ImGui::ShowDemoWindow(&menu_opts.show_imgui_demo);
    if (menu_opts.show_network_prompt)
        show_network_prompt(_network->net, "Network status",
                            &menu_opts.show_network_prompt);

    _start_prompt.show_window();
    _debug->unit_info->show();
    /*
        if (_system->phase == GamePhase::battles && _system->is_local_player_now()) {
            _fight_system.make_fight_stack(_system->current_player_index());
            auto actions = _fight_system.compute_fight_result();
            _fight_system.clear();
            std::move(std::begin(actions), std::end(actions),
                      std::back_inserter(_pending_actions));
            _pending_actions.push_back(std::make_unique<NextPhaseAction>());
        }

        if (_system->phase == GamePhase::new_day && _system->is_local_player_now()) {
            _system->scenario->units.apply_for_each<MovementComponent>([&](auto& mc) {
                MovementComponent new_mc = mc;
                new_mc.moving_pts        = new_mc.total_moving_pts();
                _pending_actions.push_back(
                    std::make_unique<ComponentChangeAction<MovementComponent>>(new_mc));

                return true;
            });
            _pending_actions.push_back(std::make_unique<NextPhaseAction>());
        }
    */
    _network->update();
}

void Game::render() { _system->gfx.draw(_window); }

void Game::clear_loop() { _system->clear(); }

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
            _system->gfx.layout->size.y /= map_tilt_speed;
            break;
        case sf::Keyboard::S:
            _system->gfx.layout->size.y *= map_tilt_speed;
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
    const auto coord = world_point_to_hex(position, *_system->gfx.layout);
    switch (button) {
        case sf::Mouse::Left:
            _system->handle_hex_selection(coord);
            break;
        case sf::Mouse::Right:
            _system->handle_hex_info(coord);

        default:
            break;
    }
}

void Game::mouse_button_released_event(const sf::Mouse::Button& button,
                                       const sf::Vector2f& position) {
    const auto coord = world_point_to_hex(position, *_system->gfx.layout);
    switch (button) {
        case sf::Mouse::Left:
            _system->handle_hex_release(coord);
            break;
        case sf::Mouse::Right:
            break;

        default:
            break;
    }
}

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
    const auto coord = world_point_to_hex(position, *_system->gfx.layout);
    _system->handle_hex_over(coord);
}