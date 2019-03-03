#pragma once

#include <vector>
#include <set>

#include "application.h"
#include "map/map.h"
#include "mover/mover.h"
#include "unit/unit.h"
#include "unit/stack.h"

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

    void resolve_stacks_and_units();


    std::vector<std::unique_ptr<Unit>> _units;    
    std::set<Unit*> _units_to_draw;
    std::vector<Stack> _stacks;

    Map _map;
    std::unique_ptr<Mover> _mover = nullptr;

    bool _moving = false;

    bool _moving_view_up    = false;
    bool _moving_view_down  = false;
    bool _moving_view_right = false;
    bool _moving_view_left  = false;

    tgui::Panel::Ptr _panel;

    constexpr static float _token_size         = 30;
    constexpr static float _view_moving_speed = 0.3f;
};