#pragma once

#include <unordered_set>

#include "unit.h"

class Stack : public Unit {
   public:
    void set_display_content(bool display);

    void draw(sf::RenderTarget& window) override;
    Unit_type get_type() const override;
    void set_color() override;
    Mover* get_mover() override;


   private:
    std::unordered_set<Unit*> _stack;
    bool _display_content = false;
};

inline void Stack::set_display_content(bool display) { _display_content = display; }

inline Unit_type Stack::get_type() const { return Unit_type::stack; }

inline void Stack::set_color() { _token.setFillColor(sf::Color(100, 100, 100)); }