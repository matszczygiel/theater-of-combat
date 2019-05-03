#include "game_app.h"

#include <functional>
#include <iostream>

#include "log.h"

#include "networking/client.h"
#include "networking/server.h"
#include "unit/heavy_unit.h"

void Game::initialize() {
    ENGINE_TRACE("Creating a window.");
    _window.create(sf::VideoMode(800, 600), "Theater of combat");
    _window.setFramerateLimit(60);

    auto view = _window.getView();
    view.setViewport(sf::FloatRect(0.25, 0, 1, 1));
    _window.setView(view);

    //    _map = Map::create_test_map(token_size);
    //    _map.save_map("resources/maps/test_map.xml");
    _map.load_map("resources/maps/test_map.xml", _token_size);
    _map.set_numbers_drawing("resources/fonts/OpenSans-Regular.ttf");

    Tokenizable::load_textures("resources/textures/units.png");
    Unit::load_font_file("resources/fonts/OpenSans-Regular.ttf");

    GAME_INFO("Initializing units.");
    _units.emplace_back(std::make_unique<Mechanized>());
    _units.emplace_back(std::make_unique<Armoured_cavalary>());
    _units.emplace_back(std::make_unique<Armoured_cavalary>());
    for (auto& u : _units)
        u->init_token(_token_size);

    _units[0]->place_on_hex(_map.get_hex(56).get());
    _units[1]->place_on_hex(_map.get_hex(19).get());
    _units[2]->place_on_hex(_map.get_hex(4).get());

    _players[0].set_name("Player 0");
    _players[1].set_name("Player 1");

    _players[0].add_unit(_units[0].get());
    _players[0].add_unit(_units[1].get());
    _players[1].add_unit(_units[2].get());
    _current_player = _players.begin();
    _resolve_units  = true;

    _running = true;

    _panel = tgui::Panel::create();
    _gui.add(_panel, "panel");
    _panel->setPosition(0, 0);
    _panel->setSize("25%", "100%");
    _panel->getRenderer()->setBackgroundColor(sf::Color::Blue);

    auto label = tgui::Label::create("Selected units");
    _panel->add(label, "label");
    label->setTextSize(25);
    /*
    char c;
    std::cout << "server [s] or client [c] ?\n";
    std::cin >> c;
    if (c == 's') {
        unsigned short port;
        std::cout << "enter port\n";
        std::cin >> port;
        Server::listen_at_port(port);
        std::cout << "local ip  :" << Server::get_local_ip() << '\n';
        std::cout << "public ip :" << Server::get_public_ip() << '\n';
        std::cout << "port      :" << Server::get_port() << '\n';
        std::cout << "Accepting client.\n";
        Server::accept_client();
        std::cout << "Client accepted.\n";
    } else if (c == 'c') {
        unsigned short port;
        std::string ip;
        std::cout << "enter port\n";
        std::cin >> port;
        std::cout << "enter ip\n";
        std::cin >> ip;
        Client::connect_to_server(sf::IpAddress(ip), port);
    }
    */

    auto menu = tgui::MenuBar::create();
    _gui.add(menu, "menu");
    menu->setSize("100%", "7%");
    menu->addMenu("Network");
    menu->addMenuItem("Create server");
    menu->addMenuItem("Connect to server");
    menu->connectMenuItem("Network", "Create server", [&]() { _gui.add(Server::create_prompt_window()); });
    menu->connectMenuItem("Network", "Connect to server", [&]() { _gui.add(Client::create_prompt_window()); });
}

void Game::update(const sf::Time& elapsed_time) {
    auto view = _window.getView();
    sf::Vector2f moving_view(0, 0);
    if (_moving_view_down)
        moving_view += sf::Vector2f(0, 1);
    if (_moving_view_up)
        moving_view += sf::Vector2f(0, -1);
    if (_moving_view_right)
        moving_view += sf::Vector2f(1, 0);
    if (_moving_view_left)
        moving_view += sf::Vector2f(-1, 0);

    moving_view *= elapsed_time.asMilliseconds() * _view_moving_speed;

    view.move(moving_view);
    _window.setView(view);

    if (_resolve_units) {
        resolve_stacks_and_units(_current_player->get_players_units());
        _resolve_units = false;
    }
}

void Game::render() {
    _map.draw(_window);
    for (auto& u : _units_to_draw)
        u->draw(_window);

    for (auto& s : _stacks)
        s.draw(_window);
}

void Game::finalize() {
}

void Game::key_pressed_event(const sf::Keyboard::Key& key) {
    switch (key) {
        case sf::Keyboard::Escape:
            _running = false;
            break;

        case sf::Keyboard::R:
            if (++_current_player == _players.end()) {
                _current_player = _players.begin();
                std::for_each(_units.begin(), _units.end(), [](auto& u) { u->reset_mv_points(); });
            }
            _resolve_units = true;
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
            if (!_moving) {
                for (auto& u : _units_to_draw) {
                    if (u->token_contains(position)) {
                        init_mover_and_info_for_unit(u);
                        break;
                    }
                }
                if (!_moving) {
                    for (auto& s : _stacks) {
                        if (s.token_contains(position)) {
                            _panel->add(s.create_displayer([&](Unit* u) { this->init_mover_and_info_for_unit(u); }),
                                        "unit info");
                            break;
                        }
                    }
                }
            } else {
                _mover->move(position);
                _moving = false;
                _panel->remove(_panel->get("unit info"));
                _mover.reset(nullptr);
                _resolve_units = true;
            }
            break;

        default:
            break;
    }
}

void Game::mouse_button_released_event(const sf::Mouse::Button& button,
                                       const sf::Vector2f& position) {
}

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
    _gui.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(width), static_cast<float>(height))));
}

void Game::resolve_stacks_and_units(std::set<Unit*>& unit_set) {
    _stacks.clear();
    _units_to_draw.clear();

    for (auto& u : unit_set) {
        auto occ           = u->get_ocupation();
        bool stack_created = false;

        for (auto it = _units_to_draw.begin(); it != _units_to_draw.end();) {
            auto unit = *it;
            if (unit->get_ocupation() == occ) {
                if (!stack_created) {
                    _stacks.emplace_back(Stack());
                    _stacks.back().add_unit(u);
                    _stacks.back().init_token(_token_size);
                    _stacks.back().set_token_postion(occ->get_position());
                    stack_created = true;
                }
                _stacks.back().add_unit(unit);
                it = _units_to_draw.erase(it);
            } else {
                ++it;
            }
        }

        if (!stack_created)
            _units_to_draw.insert(u);
    }
}

void Game::init_mover_and_info_for_unit(Unit* unit) {
    _panel->remove(_panel->get("unit info"));
    _mover.reset(unit->get_mover());
    _mover->find_paths();
    _moving = true;
    _panel->add(unit->create_displayer(), "unit info");
}