#pragma once

#include "mover.h"

class Tracks_mover : public Mover {
   public:
    explicit Tracks_mover(Unit* unit) : Mover(unit) {}

   private:
    void create_table() override {
        _hex_table[Hex_type::field]     = 2;
        _hex_table[Hex_type::forest]    = 4;
        _hex_table[Hex_type::hills]     = 3;
        _hex_table[Hex_type::mountains] = 5;
        _hex_table[Hex_type::swamp]     = 7;

        _pass_table[Passage_type::stream]     = 2;
        _pass_table[Passage_type::creek]      = 3;
        _pass_table[Passage_type::river]      = 5;
        _pass_table[Passage_type::huge_river] = 8;
        _pass_table[Passage_type::road]       = 1;
    }
};
