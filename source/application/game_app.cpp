#include "game_app.h"

#include <fstream>
#include <functional>
#include <iostream>

#include <cereal/archives/xml.hpp>

#include "log.h"

#include "messaging/concrete_message.h"
#include "messaging/messaging.h"
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

    // _map = Map::create_test_map(_token_size);
    /*
   {
        std::ofstream os("resources/maps/cereal_map.xml");
        cereal::XMLOutputArchive ar(os);
        ar(_map);
    }
*/

    {
        std::ifstream is("resources/maps/cereal_map.xml");
        cereal::XMLInputArchive iar(is);
        iar(_map);
    }

    _map.recompute_geometry(_token_size);
    _map.set_numbers_drawing("resources/fonts/OpenSans-Regular.ttf");

    Tokenizable::load_textures("resources/textures/units.png");
    Unit::load_font_file("resources/fonts/OpenSans-Regular.ttf");

    GAME_INFO("Initializing units.");

    _players[0].teams.emplace_back(Team("team 0", _map, _unit_set));
    _players[1].teams.emplace_back(Team("team 1", _map, _unit_set));

    auto u0 = _unit_set.push_unit<Mechanized>("unit 0");
    auto u1 = _unit_set.push_unit<Armoured_cavalry>("unit 1");
    auto u2 = _unit_set.push_unit<Armoured_cavalry>("unit 2");

    _players[0].teams[0].add_unit_ownership(u0);
    _players[0].teams[0].add_unit_ownership(u1);
    _players[1].teams[0].add_unit_ownership(u2);

    _unit_set.init_tokens(_token_size);

    _unit_set.get_by_id(u0)->place_on_hex(&_map.get_hex(56));
    _unit_set.get_by_id(u1)->place_on_hex(&_map.get_hex(19));
    _unit_set.get_by_id(u2)->place_on_hex(&_map.get_hex(4));

    _players[0].set_name("Player 0");
    _players[1].set_name("Player 1");

    _current_player = _players.begin();

    _running = true;

    _message_bus->add_listener(
        Message::get_id<Unit_moved_msg>(), [&](Message::ptr_base& msg) {
            auto um_msg = static_cast<Unit_moved_msg*>(msg.get());
            auto u      = _unit_set.get_by_id(um_msg->unit_id);
            u->place_on_hex(&_map.get_hex(um_msg->dest_id));
            u->reduce_mv_points(um_msg->cost);

            auto hostiles = other_player()->teams[0].get_units_controling(
                u->get_occupation()->get_number());
            if (hostiles.empty())
                return;

            auto& bf = _battlefields.emplace_back(Battlefield());
            bf.push(hostiles, other_player()->name());
            bf.push(u, _current_player->name());
        });
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

    _current_player->teams[0].update();

    /*    if (_resolve_units) {
        resolve_stacks_and_units(_current_player->get_players_units());
        _resolve_units = false;
    }
*/
    // Networking
    switch (_network_status) {
        case Network_status::sever_accepting:
            if (std::get<Server>(_network).accept_client()) {
                _network_status = Network_status::server;
                GAME_INFO("Connected client.");
            }
            break;
        default:
            break;
    }
}

void Game::render() {
    _map.draw(_window);
    _current_player->teams[0].draw(_window);
}

void Game::finalize() {}

void Game::key_pressed_event(const sf::Keyboard::Key& key) {
    switch (key) {
        case sf::Keyboard::Escape:
            _running = false;
            break;

        case sf::Keyboard::R:
            _current_player = other_player();
            for (auto& b : _battlefields)
                b.carry_fight(_message_bus);

            _battlefields.clear();

            if (_current_player == _players.begin()) {
                _unit_set.apply([](Unit& u) { u.reset_mv_points(); });
            }
            //            _resolve_units = true;
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
                _current_player->teams[0].apply([&](Unit& u) {
                    if (u.token_contains(position)) {
                        init_mover_and_info_for_unit(u);
                    }
                });
            }
            /*           if (!_moving) {
                for (auto& s : _stacks) {
                    if (s.token_contains(position)) {
                        _panel->add(s.create_displayer([&](std::shared_ptr<Unit>
    u) { this->init_mover_and_info_for_unit(u); }), "unit info"); break;
                    }
                }
            }
    }*/
            else {
                _mover->move(position, {}, _message_bus);
                _moving = false;
                _mover.reset(nullptr);
                //                _resolve_units = true;
            }
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
/*
void Game::resolve_stacks_and_units(std::set<std::shared_ptr<Unit> >& unit_set)
{ _stacks.clear(); _units_to_draw.clear();

    for (auto& u : unit_set) {
        auto occ           = u->get_occupation();
        bool stack_created = false;

        for (auto it = _units_to_draw.begin(); it != _units_to_draw.end();) {
            auto unit = *it;
            if (unit->get_occupation() == occ) {
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
*/
void Game::init_mover_and_info_for_unit(Unit& unit) {
    _mover = unit.get_mover(_map);
    _mover->find_paths();
    _moving = true;
}

std::array<Player, 2>::iterator Game::other_player() {
    std::array<Player, 2>::iterator res = _current_player + 1;
    if (res == _players.end()) {
        return _players.begin();
    }

    return res;
}