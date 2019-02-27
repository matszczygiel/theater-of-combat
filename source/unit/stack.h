#pragma once

#include <vector>

#include "unit.h"

class Stack {
   public:
    void set_display_content(bool display);

   private:
    std::vector<Unit*> _stack;
    bool _display_content = false;
};

inline void Stack::set_display_content(bool display) { _display_content = display; }