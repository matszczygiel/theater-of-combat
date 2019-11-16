#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "toc/unit/unit.h"
#include "toc/unit/unit_components.h"
#include "toc/unit/unit_manager.h"

TEST_CASE("UnitManager") {
    SUBCASE("assigning and removing") {
        UnitManager um{};
        const auto u0 = um.create(0, "test unit 0");
        const auto u1 = um.create(0, "test unit 1");
        const auto u2 = um.create(1, "test unit 2");
        const auto u3 = um.create(1, "test unit 3");

        CHECK_EQ(u0, 0);
        CHECK_EQ(u1, 1);
        CHECK_EQ(u2, 2);
        CHECK_EQ(u3, 3);

        CHECK_EQ(um.units().at(u0).name(), "test unit 0");

        CHECK_THROWS(um.assign_component<PositionComponent>(4));
        CHECK_THROWS(um.get_component<PositionComponent>(4));
        CHECK_THROWS(um.remove_component<PositionComponent>(4));

        CHECK_THROWS(um.assign_component<PositionComponent>(u0));

        CHECK_NOTHROW(um.remove_component<PositionComponent>(u3));
        CHECK_NOTHROW(um.assign_component<PositionComponent>(u3));

        CHECK_NOTHROW(um.remove_component<PositionComponent>(u3));
        CHECK_FALSE(um.get_component<PositionComponent>(u3));

        auto mc1 = um.get_component<PositionComponent>(u1);
        CHECK(mc1);
        CHECK_EQ(mc1->owner(), u1);
        CHECK_EQ(mc1->owner_type(), 0);
        CHECK_FALSE(mc1->position);
        CHECK_FALSE(mc1->direction);
    }

    SUBCASE("for each") {
        UnitManager um{};
        std::vector<Unit::IdType> vec{};
        for (int i = 0; i < 4; ++i) {
            vec.push_back(um.create(0, "test"));
        }

        um.apply_for_each<PositionComponent>([](PositionComponent& mc) {
            mc.position = HexCoordinate(1, 2);
            return true;
        });

        for (const auto& id : vec) {
            CHECK_EQ(HexCoordinate(1, 2),
                     um.get_component<PositionComponent>(id)->position);
        }

        um.apply_for_each<PositionComponent>([](PositionComponent& mc) {
            if (mc.owner() == 2) {
                mc.position = HexCoordinate(0, 0);
                return false;
            }
            return true;
        });

        for (const auto& id : vec) {
            if (id == 2) {
                CHECK_EQ(HexCoordinate(0, 0),
                         um.get_component<PositionComponent>(id)->position);
            } else {
                CHECK_EQ(HexCoordinate(1, 2),
                         um.get_component<PositionComponent>(id)->position);
            }
        }
    }
}
