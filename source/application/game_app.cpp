#include "game_app.h"

#include "log.h"

#include "unit/heavy_unit.h"

void Game::initialize() {
    ENGINE_TRACE("Creating a window.");
    _window.create(sf::VideoMode(800, 600), "Theater of combat");
    _window.setFramerateLimit(60);

    _map = Map::create_test_map(token_size);

    GAME_INFO("Initializing units.");
    _units.emplace_back(new Mechanized());
    _units.emplace_back(new Armoured_cavalary());
    for (auto& u : _units)
        u->init_token(token_size);

    _units[0]->place_on_hex(_map.get_hex(34));
    _units[1]->place_on_hex(_map.get_hex(19));

    _running = true;
}

void Game::handle_event(const sf::Event& event) {
    const sf::Vector2f mouse_pos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

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
                case sf::Mouse::Left:
                    for (auto& u : _units)
                        if (u->token_contain(mouse_pos)) {
                            _mover.set_unit(u);
                            break;
                        }
                    break;

                default:
                    break;
            }
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button) {
                case sf::Mouse::Left:
                    _mover.move(mouse_pos);
                    break;

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