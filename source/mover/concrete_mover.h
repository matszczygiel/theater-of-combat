#pragma once

#include "mover.h"

class Tracks_mover : public Mover {
   public:
    explicit Tracks_mover(Unit* unit) : Mover(unit) {}

   private:
    void create_table() override {
        _table[Site_type::field]     = 2;
        _table[Site_type::forest]    = 4;
        _table[Site_type::hills]     = 3;
        _table[Site_type::mountains] = 5;
        _table[Site_type::swamp]     = 7;

        _table[Site_type::stream]     = 2;
        _table[Site_type::creek]      = 3;
        _table[Site_type::river]      = 5;
        _table[Site_type::huge_river] = 8;
        _table[Site_type::road]       = 1;
    }
};
