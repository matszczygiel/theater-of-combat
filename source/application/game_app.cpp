#include "game_app.h"

#include "log.h"

#include "unit/heavy_unit.h"

void Game::initialize() {
    ENGINE_TRACE("Creating a window.");
    _window.create(sf::VideoMode(800, 600), "Theater of combat");
    _window.setFramerateLimit(60);

//    _map = Map::create_test_map(token_size);
//    _map.save_map("resources/maps/test_map.xml");
    _map.load_map("resources/maps/test_map.xml", token_size);

    GAME_INFO("Initializing units.");
    _units.emplace_back(new Mechanized());
    _units.emplace_back(new Armoured_cavalary());
    for (auto& u : _units)
        u->init_token(token_size);

    _units[0]->place_on_hex(_map.get_hex(56).get());
    _units[1]->place_on_hex(_map.get_hex(19).get());

    _running = true;
}

void Game::handle_event(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed:
            _running = false;
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    _running = false;
                    break;

                case sf::Keyboard::R:
                    for (auto& u : _units)
                        u->reset_mv_points();
                    break;

                default:
                    break;
            }
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button) {
                case sf::Mouse::Left: {
                    const sf::Vector2f mouse_pos =
                        _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                    if (!_moving) {
                        for (auto& u : _units) {
                            if (u->token_contain(mouse_pos)) {
                                _mover = u->get_mover();
                                _mover->find_paths();
                                _moving = true;
                                break;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button) {
                case sf::Mouse::Left: {
                    const sf::Vector2f mouse_pos =
                        _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    if (_moving) {
                        _mover->move(mouse_pos);
                        _moving = false;
                        delete _mover;
                    }
                    break;
                }
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void Game::update() {}

void Game::render() {
    _window.clear(sf::Color::Blue);

    _map.draw(_window);
    for (auto& u : _units)
        u->draw(_window);

    _window.display();
}

void Game::finalize() {
    for (auto& u : _units)
        delete u;
}