#ifndef GAME_APP_H
#define GAME_APP_H

#include <variant>

#include "toc/core/application.h"
#include "toc/core/resource_loader.h"
#include "toc/gui/console.h"
#include "toc/gui/log_window.h"
#include "toc/map/map.h"
#include "toc/unit/unit.h"
#include "toc/graphics/graphics_state.h"
#include "toc/gameplay/system_state.h"

#include "gui/start_prompt.h"
#include "networking/net_manager.h"
#include "systems/debug_info.h"
#include "systems/fighter.h"
#include "systems/mover.h"

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
    void window_resize_event(const unsigned& width, const unsigned& height) final;
    void mouse_moved_event(const sf::Vector2f& position) final;

   private:
    bool _moving_view_up{false};
    bool _moving_view_down{false};
    bool _moving_view_right{false};
    bool _moving_view_left{false};

    std::string _local_player_name{};
    std::shared_ptr<NetManager> _network{std::make_shared<NetManager>()};

    std::shared_ptr<ResourceLoader> _res_loader{
        std::make_shared<ResourceLoader>("resources/")};

    std::unique_ptr<SystemState> _system;


    enum class PacketHeader : sf::Int8 { none = 0, action = 1, take_turn = 2 };
    
    LogWindow _log{"Log console"};
    ConsoleWindow _console{"Lua console"};
    StartPrompt _start_prompt{_network, _res_loader};

    constexpr static float _view_moving_speed{0.3f};
};

#endif