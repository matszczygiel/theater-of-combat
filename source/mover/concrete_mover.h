#pragma once

#include "mover.h"

class Tracks_mover : public Mover {
   public:
    explicit Tracks_mover(std::shared_ptr<Unit> unit, std::shared_ptr<Map> map) : Mover(unit, map) {}

   private:
    std::unordered_map<Map_site::id_type, int> create_table() const override {
        std::unordered_map<Map_site::id_type, int> table;
        
        table["Field"]     = 2;
        table["Forest"]    = 4;

        table["River"]    = 4;

        return table;
    }
};
