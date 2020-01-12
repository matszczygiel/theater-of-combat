#ifndef KIRCH_CHASE_H
#define KIRCH_CHASE_H

#include <optional>
#include <vector>

#include "kirch_component_system.h"
#include "kirch_components.h"
#include "kirch_fight.h"
#include "kirch_mover.h"

namespace kirch {

class ChaseSystem : public ComponentSystemKircholm {
   public:
    enum class State {
        done,
        data_set,
        data_selected,
        unit_selected,
        destination_selected,
        waiting,
    };

    explicit ChaseSystem(SystemKircholm* system) noexcept;

    void on_left_click(HexCoordinate coord);
    void on_right_click(HexCoordinate coord);
    void on_left_realease(HexCoordinate coord);
    void update();

    void set_results(const std::vector<DirectFightResult>& results);
    void set_chase_done(int index);

   private:
    void render_ui();
    void render_done_button();

    State select_data(int index);
    State select_unit(HexCoordinate coord);
    State unit_selected(HexCoordinate coord);
    State select_direction(HexCoordinate coord);

   private:
    std::vector<DirectFightResult> _results{};
    int _current_data{};
    std::set<int> _fights_to_process{};
    std::set<int> _fights_to_wait{};

    std::map<HexCoordinate, const PositionComponent*> _units_to_move{};
    const PositionComponent* _current_pc{nullptr};
    HexCoordinate _destination{};

    State _state{State::done};
};
}  // namespace kirch

#endif /* KIRCH_CHASE_H */
