#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H

#include <memory>

#include "toc/gui/unit_info.h"

class Action;

class DebugInfoSystem {
   public:
    void log_action(const std::unique_ptr<Action>& action) const;

    std::unique_ptr<UnitInfo> unit_info{nullptr};
};
#endif