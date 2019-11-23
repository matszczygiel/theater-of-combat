#ifndef EDITOR_APP_H
#define EDITOR_APP_H

#include "toc/core/application.h"
#include "toc/core/resource_loader.h"
#include "toc/gameplay/action.h"
#include "toc/gameplay/scenario.h"
#include "toc/gameplay/system_state.h"
#include "toc/graphics/graphics_state.h"
#include "toc/gui/console.h"
#include "toc/gui/log_window.h"
#include "toc/map/map.h"
#include "toc/unit/unit.h"

#include "gui/map_creator.h"
#include "gui/system_select.h"

class Editor : public Application {
   public:
    Editor();

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
    ScenarioDescription _scenario_desc{};

    bool _moving_view_up{false};
    bool _moving_view_down{false};
    bool _moving_view_right{false};
    bool _moving_view_left{false};
    constexpr static float _view_moving_speed{0.3f};

    std::shared_ptr<ResourceLoader> _res_loader{
        std::make_shared<ResourceLoader>("resources/")};

    std::unique_ptr<SystemState> _system{nullptr};
    std::shared_ptr<DebugInfoSystem> _debug{nullptr};

    LogWindow _log{"Log console"};
    ConsoleWindow _console{"Lua console"};
    SystemSelector _sys_selector{"System selector"};
    MapCreator _map_creator{"Map creator"};
};

#endif