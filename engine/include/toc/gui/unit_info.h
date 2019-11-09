#ifndef UNIT_INFO_H
#define UNIT_INFO_H

#include "toc/unit/unit_manager.h"
#include "toc/gameplay/scenario.h"

class UnitInfo {
   public:
    UnitInfo(const std::shared_ptr<Scenario> scenario);
    virtual ~UnitInfo() = default;
    virtual void draw_component_trees();

    void set_current_unit_id(Unit::IdType id);
    void show();

   protected:
    const std::shared_ptr<Scenario> _scenario{nullptr};

   private:
    Unit::IdType _current_unit_id{};
    Unit _current_unit{};
    bool _open{false};
};

#endif