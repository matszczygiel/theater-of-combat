#pragma once

#include "mover.h"

class Tracks_mover : public Mover {
   public:
    explicit Tracks_mover(Unit* unit, Map& map) : Mover(unit, map) {}

   private:
    std::unordered_map<Hex_site::Type, int> create_hex_table() const override {
        std::unordered_map<Hex_site::Type, int> table;

        table[Hex_site::Type::Field]  = 2;
        table[Hex_site::Type::Forest] = 4;

        return table;
    }

    std::unordered_map<Passage_site::Type, int> create_pass_table() const override {
        std::unordered_map<Passage_site::Type, int> table;

        table[Passage_site::Type::River] = 2;

        return table;
    }
};
