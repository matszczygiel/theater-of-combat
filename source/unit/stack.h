#pragma once

#include <unordered_set>

#include "tokenizable.h"
#include "unit.h"

class Stack : public Tokenizable {
   public:
    void set_display_content(bool display);

    void draw(sf::RenderTarget& target) const override;
    Mover* get_mover();
    int texture_offset() const final;
    int size() const;

    void add_unit(Unit* unit);
    void remove_unit(Unit* unit);

   private:
    std::unordered_set<Unit*> _stack;
    bool _display_content = false;
};

inline void Stack::set_display_content(bool display) { _display_content = display; }

inline int Stack::texture_offset() const { return 2; }

inline void Stack::add_unit(Unit* unit) { _stack.insert(unit); }

inline void Stack::remove_unit(Unit* unit) { _stack.erase(unit); }

inline int Stack::size() const { return _stack.size(); }
