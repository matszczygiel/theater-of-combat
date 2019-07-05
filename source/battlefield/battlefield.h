#pragma once

#include <map>
#include <set>
#include <memory>

#include "unit/unit.h"

class Message_bus;

class Battlefield {
   public:
    void push(Unit* unit, const std::string& owner_name);
    void push(std::set<Unit*> units, const std::string& owner_name);

    void carry_fight(std::shared_ptr<Message_bus>& bus);

   private:
    std::map<std::string, std::set<Unit*>> _bucket;
};
