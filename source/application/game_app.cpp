#include "game_app.h"

#include "log.h"

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

    GAME_INFO("Initializing units.");
    _units.emplace_back(std::make_unique<Mechanized>());
    _units.emplace_back(std::make_unique<Armoured_cavalary>());
    for (auto& u : _units)
        u->init_token(_token_size);

    _units[0]->place_on_hex(_map.get_hex(56).get());
    _units[1]->place_on_hex(_map.get_hex(19).get());

    _running = true;

    _panel = tgui::Panel::create();
    _gui.add(_panel, "panel");
    _panel->setPosition(0, 0);
    _panel->setSize("25%", "100%");
    _panel->getRenderer()->setBackgroundColor(sf::Color::Blue);

    auto label = tgui::Label::create("dupa");
    _gui.add(label, "label");
    label->setTextSize(25);
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

    resolve_stacks_and_units();
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
            for (auto& u : _units)
                u->reset_mv_points();
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
                for (auto& u : _units) {
                    if (u->token_contains(position)) {
                        _mover.reset(u->get_mover());
                        _mover->find_paths();
                        _moving = true;
                        u->create_displayer(_panel, "unit info");
                        break;
                    }
                }
            } else {
                _mover->move(position);
                _moving = false;
                _panel->remove(_panel->get("unit info"));
                _mover.reset(nullptr);
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
}

void Game::resolve_stacks_and_units() {
    _stacks.clear();
    _units_to_draw.clear();

    for (auto& u : _units) {
        auto occ           = u->get_ocupation();
        bool stack_created = false;

        for (auto it = _units_to_draw.begin(); it != _units_to_draw.end();) {
            auto unit = *it;
            if (unit->get_ocupation() == occ) {
                if (!stack_created) {
                    _stacks.emplace_back(Stack());
                    _stacks.back().add_unit(u.get());
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
            _units_to_draw.insert(u.get());
    }
}