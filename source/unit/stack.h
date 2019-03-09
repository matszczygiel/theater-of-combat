#pragma once

#include <unordered_set>

#include <TGUI/TGUI.hpp>

#include "tokenizable.h"
#include "unit.h"

class Stack : public Tokenizable {
   public:
    virtual ~Stack() = default;

    void set_display_content(bool display);
    void draw(sf::RenderTarget& target) const override;
    int texture_offset() const override;
    Mover* get_mover();
    int size() const;
    tgui::Grid::Ptr create_displayer(const std::function<void(Unit*)>& widget_callback) const;

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
