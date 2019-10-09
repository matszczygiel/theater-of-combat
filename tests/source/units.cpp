#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "unit/unit.h"
#include "unit/unit_components.h"
#include "unit/unit_manager.h"

TEST_CASE("Components") {
    SUBCASE("MovementComponent") {
        CHECK_NOTHROW(MovementComponent mc1(0));
        CHECK_NOTHROW(MovementComponent mc2(56));
        CHECK_THROWS(MovementComponent mc3(-1));
    }
}

TEST_CASE("UnitManager") {
    SUBCASE("assigning and removing") {
        UnitManager um{};
        const auto u0 = um.create(UnitType::mechanized, "test unit 0", true);
        const auto u1 = um.create(UnitType::mechanized, "test unit 1", true);
        const auto u2 = um.create(UnitType::heavy, "test unit 2", true);
        const auto u3 = um.create(UnitType::heavy, "test unit 3", false);

        CHECK_EQ(u0, 0);
        CHECK_EQ(u1, 1);
        CHECK_EQ(u2, 2);
        CHECK_EQ(u3, 3);

        CHECK_EQ(um.units().at(u0).name(), "test unit 0");

        CHECK_THROWS(um.assign_component<MovementComponent>(4, 10));
        CHECK_THROWS(um.get_component<MovementComponent>(4));
        CHECK_THROWS(um.remove_component<MovementComponent>(4));

        CHECK_THROWS(um.assign_component<MovementComponent>(u0, 10));

        CHECK_NOTHROW(um.assign_component<MovementComponent>(u3));

        CHECK_FALSE(um.get_component<FightComponent>(u3));

        CHECK_NOTHROW(um.remove_component<FightComponent>(u0));

        auto mc1 = um.get_component<MovementComponent>(u1);
        CHECK(mc1);
        CHECK_EQ(mc1->owner(), u1);
        CHECK_EQ(mc1->owner_type(), UnitType::mechanized);
    }

    SUBCASE("for each") {
        UnitManager um{};
        std::vector<Unit::IdType> vec{};
        for (int i = 0; i < 4; ++i) {
            vec.push_back(um.create(UnitType::mechanized, "test", true));
        }

        um.apply_for_each<MovementComponent>([](MovementComponent& mc) {
            mc.moving_pts = 100;
            return true;
        });

        for (const auto& id : vec) {
            CHECK_EQ(100, um.get_component<MovementComponent>(id)->moving_pts);
        }

        um.apply_for_each<MovementComponent>([](MovementComponent& mc) {
            if (mc.owner() == 2) {
                mc.moving_pts = 50;
                return false;
            }
            return true;
        });

        for (const auto& id : vec) {
            if (id == 2) {
                CHECK_EQ(50, um.get_component<MovementComponent>(id)->moving_pts);
            } else {
                CHECK_EQ(100, um.get_component<MovementComponent>(id)->moving_pts);
            }
        }
    }
}
