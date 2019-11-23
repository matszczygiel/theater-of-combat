#ifndef SYSTEM_SELECT_H
#define SYSTEM_SELECT_H

#include <memory>
#include <string>

#include "toc/gameplay/system_state.h"

class SystemState;

class SystemSelector {
   public:
    SystemSelector(const std::string name) noexcept;

    bool retrieve(std::unique_ptr<SystemState>& system);

    void show(bool* open);
    bool is_ready() const;

   private:
    bool _ready{false};
    int _selected{0};

    std::string _name{};
};

#endif /* SYSTEM_SELECT_H */
