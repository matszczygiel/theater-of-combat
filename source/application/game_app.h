#ifndef GAME_APP_H
#define GAME_APP_H

#include <set>
#include <variant>
#include <vector>

#include "application.h"
#include "battlefield/battlefield.h"
#include "map/map.h"
#include "mover/mover.h"
#include "networking/client.h"
#include "networking/netwoking_status.h"
#include "networking/server.h"
#include "player.h"
#include "unit/stack.h"
#include "unit/unit.h"
#include "unit/unit_set.h"

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
    void window_resize_event(const unsigned& width,
                             const unsigned& height) final;

    //   void resolve_stacks_and_units(std::set<std::shared_ptr<Unit> >&
    //   unit_set);
    void init_mover_and_info_for_unit(Unit& unit);

    std::array<Player, 2>::iterator other_player();

   private:
    std::array<Player, 2> _players;
    std::array<Player, 2>::iterator _current_player;

    std::vector<Battlefield> _battlefields;

    Map _map;
    std::unique_ptr<Mover> _mover{nullptr};
    Unit_set _unit_set;

    bool _moving = false;

    bool _moving_view_up    = false;
    bool _moving_view_down  = false;
    bool _moving_view_right = false;
    bool _moving_view_left  = false;

    constexpr static float _token_size        = 30;
    constexpr static float _view_moving_speed = 0.3f;

    Network_status _network_status = Network_status::unspecified;
    std::variant<Server, Client> _network;
};

#endif