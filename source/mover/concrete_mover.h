#pragma once

#include "mover.h"

class Tracks_mover : public Mover {
   public:
    explicit Tracks_mover(std::shared_ptr<Unit> unit, std::shared_ptr<Map> map) : Mover(unit, map) {}

   private:
    std::unordered_map<Map_site::Type, int> create_table() const override {
        std::unordered_map<Map_site::Type, int> table;

        table[Map_site::Type::Field]  = 2;
        table[Map_site::Type::Forest] = 4;

        table[Map_site::Type::River] = 4;

        return table;
    }
};
