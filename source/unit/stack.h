#pragma once

#include <unordered_set>

#include "unit.h"

class Stack : public Unit {
   public:
    void set_display_content(bool display);

    void draw(sf::RenderTarget& window) const override;
    Unit_type get_type() const override;
    Mover* get_mover() override {}


   private:
    std::unordered_set<Unit*> _stack;
    bool _display_content = false;
};

inline void Stack::set_display_content(bool display) { _display_content = display; }

inline Unit_type Stack::get_type() const { return Unit_type::stack; }
