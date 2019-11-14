#ifndef KIRCH_GUI_H
#define KIRCH_GUI_H

#include "toc/gui/unit_info.h"

namespace kirch {
class KircholmUnitInfo : public UnitInfo {
   public:
    KircholmUnitInfo(const std::shared_ptr<Scenario>& scenario);
    void draw_component_trees() final;
};
}  // namespace kirch

#endif