#ifndef GAME_APP_H
#define GAME_APP_H

#include <vector>

#include "application.h"
#include "graphics/map_gfx.h"
#include "map/map.h"
#include "gui/log_window.h"
#include "gui/console.h"

class Game : public Application {
   public:
    Game();

   private:
    void initialize() final;
    void update(const sf::Time& elapsed_time) final;
    void finalize() final;
    void clear_loop() final;
    void render() final;

    void key_pressed_event(const sf::Keyboard::Key& key) final;
    void key_released_event(const sf::Keyboard::Key& key) final;
    void mouse_button_pressed_event(const sf::Mouse::Button& button,
                                    const sf::Vector2f& position) final;
    void mouse_button_released_event(const sf::Mouse::Button& button,
                                     const sf::Vector2f& position) final;
    void mouse_wheel_scrolled_event(const float& delta) final;
    void window_resize_event(const unsigned& width,
                             const unsigned& height) final;
    void mouse_moved_event(const sf::Vector2f& position) final;

   private:
    Map _map{};
    std::vector<sf::ConvexShape> _highlighted_hexes{};
    MapGfx _map_gfx{};

    bool _moving_view_up{false};
    bool _moving_view_down{false};
    bool _moving_view_right{false};
    bool _moving_view_left{false};

    LogWindow _log{"Log console"};
    ConsoleWindow _console{"Lua console"};

    constexpr static float _view_moving_speed{0.3f};
};

#endif