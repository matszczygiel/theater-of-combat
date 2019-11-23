#include "editor_app.h"

#include <imgui.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/memory.hpp>
#include <sol/sol.hpp>

#include "toc/core/log.h"
#include "toc/core/lua_vm.h"
#include "toc/gameplay/action.h"
#include "toc/gameplay/lua_gameplay.h"
#include "toc/gui/dock_space.h"
#include "toc/gui/log_window.h"
#include "toc/map/lua_map.h"
#include "toc/unit/lua_units.h"
#include "toc/unit/unit_components.h"
#include "toc/unit/unit_manager.h"

#include "gui/menu_bar.h"

Editor::Editor() {
    auto rot_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "tmp/logs/log.txt", 1048576 * 5, 2);
    rot_sink->set_pattern("%^[%c] (thread %t) %n: [%l] %v%$");
    rot_sink->set_level(spdlog::level::trace);
    logger::get_distributing_sink()->add_sink(rot_sink);
    logger::get_distributing_sink()->add_sink(
        std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());

    _res_loader->register_resource_type<Map>("maps", "map");
    _res_loader->register_resource_type<UnitManager>("units", "umg");
    _res_loader->register_resource_type<sf::Font>("fonts", "ttf");
    _res_loader->register_resource_type<sf::Texture>("textures", "png");
    _res_loader->register_resource_type<Scenario>("scenarios", "scn");
}

void Editor::initialize() {
    engine_trace("Creating a window.");
    _window.create(sf::VideoMode(1200, 800), "Theater of combat - Editor");
    _window.setFramerateLimit(60);
}

void Editor::update(const sf::Time& elapsed_time) {
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
    if (menu_opts.show_system_select) {
        _sys_selector.show(&menu_opts.show_system_select);
    }
    if (menu_opts.show_map_creator) {
        _map_creator.show(&menu_opts.show_map_creator);
    }

    if (_sys_selector.is_ready()) {
        const auto res               = _sys_selector.retrieve(_system);
        menu_opts.show_system_select = !res;
        if (res) {
            _debug = _system->create_debug_info();
        }
        _system->gfx.font =
            _res_loader->load<decltype(_system->gfx.font)>("OpenSans-Regular");
    }

    if (_map_creator.is_ready()) {
        const auto res             = _map_creator.retrieve(_system);
        menu_opts.show_map_creator = !res;
        auto view                  = _window.getView();
        view.setCenter(0.0f, 0.0f);
        _window.setView(view);
    }

    if (_system) {
        _system->update();
    }
    if (_debug)
        _debug->unit_info->show();
    // engine_trace("Updating");
}

void Editor::render() {
    if (_system)
        _system->gfx.draw(_window);
}

void Editor::clear_loop() {}

void Editor::finalize() {}

void Editor::key_pressed_event(const sf::Keyboard::Key& key) {
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
            break;
        case sf::Keyboard::S:
            break;
        default:
            break;
    }
}

void Editor::key_released_event(const sf::Keyboard::Key& key) {
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

void Editor::mouse_button_pressed_event(const sf::Mouse::Button& button,
                                        const sf::Vector2f& position) {
    switch (button) {
        case sf::Mouse::Left: {
        } break;
        case sf::Mouse::Right: {
        }
        default:
            break;
    }
}

void Editor::mouse_button_released_event(const sf::Mouse::Button&, const sf::Vector2f&) {}

void Editor::mouse_wheel_scrolled_event(const float& delta) {
    auto view                   = _window.getView();
    constexpr float zoom_factor = 1.05;
    if (delta > 0)
        view.zoom(1.0 / zoom_factor);
    else if (delta < 0)
        view.zoom(zoom_factor);

    _window.setView(view);
}

void Editor::window_resize_event(const unsigned& width, const unsigned& height) {
    auto view = _window.getView();
    view.setSize(static_cast<float>(width), static_cast<float>(height));
    _window.setView(view);
}

void Editor::mouse_moved_event(const sf::Vector2f& position) {}