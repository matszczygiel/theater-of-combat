#pragma once

#include <vector>

#include "application.h"
#include "map/map.h"
#include "mover/mover.h"
#include "unit/unit.h"

class Game : public Application {
   private:
    void initialize() final;
    void update(const sf::Time& elapsed_time) final;
    void finalize() final;
    void render() final;

    void key_pressed_event(const sf::Keyboard::Key& key) final;
    void key_released_event(const sf::Keyboard::Key& key) final;
    void mouse_button_pressed_event(const sf::Mouse::Button& button,
                                    const sf::Vector2f& position) final;
    void mouse_button_released_event(const sf::Mouse::Button& button,
                                     const sf::Vector2f& position) final;
    void mouse_wheel_scrolled_event(const float& delta) final;
    void window_resize_event(const unsigned& width, const unsigned& height) final;

    std::vector<Unit*> _units;
    Map _map;
    Mover* _mover = nullptr;

    bool _moving = false;

    bool _moving_view_up    = false;
    bool _moving_view_down  = false;
    bool _moving_view_right = false;
    bool _moving_view_left  = false;

    constexpr static float token_size         = 30;
    constexpr static float _view_moving_speed = 0.3f;
};