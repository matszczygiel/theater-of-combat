#ifndef UNIT_H
#define UNIT_H

#include <string>

#include <entityx/Entity.h>

enum class UnitType { heavy, mechanized };

class Unit {
   public:
    Unit(entityx::EntityManager &es, UnitType type, std::string name);
    ~Unit();

    Unit(const Unit &) = delete;
    Unit &operator=(const Unit &) = delete;

    const std::string name{};
    const UnitType type{};

   private:
    void create_from_type(UnitType type);

    entityx::Entity _entity{};
};

#endif