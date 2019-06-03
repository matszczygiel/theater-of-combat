#pragma once

#include <set>
#include <vector>
#include <variant>

#include "application.h"
#include "map/map.h"
#include "mover/mover.h"
#include "player.h"
#include "unit/stack.h"
#include "unit/unit.h"
#include "networking/netwoking_status.h"
#include "networking/client.h"
#include "networking/server.h"
#include "battlefield/battlefield.h"

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

    void resolve_stacks_and_units(std::set<std::shared_ptr<Unit> >& unit_set);
    void init_mover_and_info_for_unit(std::shared_ptr<Unit>  unit);

   private:
    std::vector<std::shared_ptr<Unit> > _units;
    std::set<std::shared_ptr<Unit> > _units_to_draw;
    std::vector<Stack> _stacks;

    std::array<Player, 2> _players;
    std::array<Player, 2>::iterator _current_player;

    std::vector<Battlefield> _battlefields;

    std::shared_ptr<Map> _map;
    std::unique_ptr<Mover> _mover = nullptr;

    bool _moving = false;

    bool _moving_view_up    = false;
    bool _moving_view_down  = false;
    bool _moving_view_right = false;
    bool _moving_view_left  = false;

    bool _resolve_units = false;

    tgui::Panel::Ptr _panel;

    constexpr static float _token_size        = 30;
    constexpr static float _view_moving_speed = 0.3f;

    Network_status _network_status = Network_status::unspecified;
    std::variant<Server, Client> _network;
};