#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <cereal/types/array.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/stack.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "scenario.h"
#include "toc/graphics/graphics_state.h"

class Action;

class SystemState {
   public:
    bool set_local_player(std::string name);
    void set_local_player(int index);
    bool is_local_player_now() const;
    int current_player_index() const;
    int opposite_player_index() const;
    void next_player();
    void push_action(std::unique_ptr<Action> action);
    void update();

    std::shared_ptr<Scenario> scenario{std::make_shared<Scenario>()};
    std::array<std::string, 2> player_names{};
    std::vector<std::unique_ptr<Action>> accumulated_actions{};
    GfxState gfx{scenario};

    virtual ~SystemState() = default;

    virtual void start();
    virtual void next_phase()                                   = 0;
    virtual void handle_hex_over(const HexCoordinate& hex)      = 0;
    virtual void handle_hex_selection(const HexCoordinate& hex) = 0;
    virtual void handle_hex_info(const HexCoordinate& hex)      = 0;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    friend class UndoPreviousAction;

    int _current_player_index{-1};
    std::optional<int> _local_player_index{};

    std::stack<std::unique_ptr<Action>, std::vector<std::unique_ptr<Action>>>
        _action_stack{};
};

template <class Archive>
void SystemState::serialize(Archive& archive) {
    archive(CEREAL_NVP(*scenario), CEREAL_NVP(player_names),
            CEREAL_NVP(_current_player_index), CEREAL_NVP(_action_stack));
}

#endif