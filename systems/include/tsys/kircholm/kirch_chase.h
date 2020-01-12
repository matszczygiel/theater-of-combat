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
        waiting,
        chasing_set,
        chasing_data,
        chasing_unit
    };

    explicit ChaseSystem(SystemKircholm* system) noexcept;

    void on_left_click(HexCoordinate coord);
    void on_right_click(HexCoordinate coord);
    void on_over(HexCoordinate coord);
    void on_left_realease(HexCoordinate coord);
    void on_right_realease(HexCoordinate coord);
    void update();

    void set_results(const std::vector<DirectFightResult>& results);
    void set_result_change(const DirectFightResult& result, int index);

   private:
    void render_ui();
    void render_done_button();
    WeightedUnidirectionalGraph<Map::SiteTypeId, Movability> make_weighted_graph() const;

    State select_data(int index);
    State select_unit_to_retreat(HexCoordinate coord);
    State unit_selected(HexCoordinate coord);

    State select_chase(int index);
    State select_unit_to_chase(HexCoordinate coord);
    State chase_unit(HexCoordinate coord);

    State init_chases();

   private:
    std::vector<DirectFightResult> _results{};
    int _current_data{};
    std::set<int> _fights_to_process{};
    std::set<int> _fights_to_wait{};

    std::map<HexCoordinate, const PositionComponent*> _units_to_move{};
    const PositionComponent* _current_pc{nullptr};
    PathSearcher _searcher{};
    std::set<HexCoordinate> _destinations{};

    State _state{State::done};
};
}

#endif /* KIRCH_CHASE_H */
