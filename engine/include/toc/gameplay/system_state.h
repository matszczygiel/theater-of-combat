#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include "action.h"
#include "scenario.h"

class SystemState {
   public:
    bool set_local_player(std::string name);
    void set_local_player(int index);
    bool is_local_player_now() const;
    int current_player_index() const;
    int opposite_player_index() const;
    void next_player();
    void push_action(std::unique_ptr<Action> action);

    std::shared_ptr<Scenario> scenario{std::make_shared<Scenario>()};
    std::array<std::string, 2> player_names{};

    virtual ~SystemState() = default;

    virtual void start()      = 0;
    virtual void next_phase() = 0;

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