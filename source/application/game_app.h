#ifndef GAME_APP_H
#define GAME_APP_H

#include "application.h"
#include "core/resource_manager.h"
#include "gameplay/game_state.h"
#include "graphics/graphics_state.h"
#include "gui/console.h"
#include "gui/log_window.h"
#include "map/map.h"
#include "networking/client.h"
#include "networking/server.h"
#include "systems/mover.h"
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

    LogWindow _log{"Log console"};
    ConsoleWindow _console{"Lua console"};

    ResourceManager _res_manager{"resources/"};

    std::shared_ptr<mover::MovementSystem> _moving_system{
        std::make_shared<mover::MovementSystem>(_state.scenario.units,
                                                _state.scenario.map)};

    debug_info::DebugInfoSystem debug_info{_state};

    enum class ActionProvider { local, remote };
    ActionProvider _action_provider{ActionProvider::local};

    std::string _local_player_name{"Your name"};

    std::vector<std::unique_ptr<Action>> _pending_actions{};

    enum class PacketHeader : sf::Int8 { none = 0, action = 1, take_turn = 2 };
    std::variant<Server, Client> _network;

    constexpr static float _view_moving_speed{0.3f};
};

#endif