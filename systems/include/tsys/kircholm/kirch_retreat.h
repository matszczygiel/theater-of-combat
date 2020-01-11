#ifndef KIRCH_RETREAT_H
#define KIRCH_RETREAT_H

#include <optional>
#include <vector>

#include "kirch_component_system.h"
#include "kirch_fight.h"
#include "kirch_mover.h"
#include "kirch_components.h"

namespace kirch {

class RetreatSystem : public ComponentSystemKircholm {
   public:
    enum class State { done, data_set, selected_data, selected_unit, waiting };

    explicit RetreatSystem(SystemKircholm* system) noexcept;

    void on_left_click(HexCoordinate coord);
    void on_right_click(HexCoordinate coord);
    void on_over(HexCoordinate coord);
    void on_left_realease(HexCoordinate coord);
    void on_right_realease(HexCoordinate coord);
    void update();

    void set_results(const std::vector<DirectFightResult>& results);
    void defence_finished() noexcept;
    void attack_finished() noexcept;

   private:
    void render_ui();
    WeightedUnidirectionalGraph<Map::SiteTypeId, Movability> make_weighted_graph() const;
    
    State select_data(int index);
    State select_unit_to_retreat(HexCoordinate coord);

   private:
    std::vector<DirectFightResult> _results{};
    int _current_data{};
    std::set<int> _fights_to_process{};

    std::map<HexCoordinate, const PositionComponent*> _units_to_move{};
    const PositionComponent* _current_pc{nullptr};
    PathSearcher _searcher{};
    std::set<HexCoordinate> _destinations{};

    State _state{done};
};
}  // namespace kirch

#endif /* KIRCH_RETREAT_H */
