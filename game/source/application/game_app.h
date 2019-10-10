#ifndef GAME_APP_H
#define GAME_APP_H

#include <variant>

#include "toc/core/application.h"
#include "gameplay/game_state.h"
#include "graphics/graphics_state.h"
#include "toc/gui/console.h"
#include "toc/gui/log_window.h"
#include "toc/toc/map/map.h"
#include "networking/client.h"
#include "networking/server.h"
#include "systems/mover.h"
#include "systems/fighter.h"
#include "systems/debug_info.h"
#include "unit/unit.h"

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
    GameState _state{};
    GfxState _gfx_state{_state};

    bool _moving_view_up{false};
    bool _moving_view_down{false};
    bool _moving_view_right{false};
    bool _moving_view_left{false};

    std::string _local_player_name{};

    LogWindow _log{"Log console"};
    ConsoleWindow _console{"Lua console"};

    ResourceLoader _res_loader{"resources/"};

    mover::MovementSystem _moving_system{_state};
    FightingSystem _fight_system{_state};

    debug_info::DebugInfoSystem debug_info{_state};

    std::vector<std::unique_ptr<Action>> _pending_actions{};

    enum class PacketHeader : sf::Int8 { none = 0, action = 1, take_turn = 2 };
    std::variant<Server, Client> _network;

    constexpr static float _view_moving_speed{0.3f};
};

#endif