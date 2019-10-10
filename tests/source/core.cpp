#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "toc/core/id_gen.h"

TEST_CASE("IdGenerator") {
    SUBCASE("generating ids") {
        IdGenerator<int> gen{};
        const auto id0 = gen.fetch();
        const auto id1 = gen.fetch();
        const auto id2 = gen.fetch();
        const auto id3 = gen.fetch();

        CHECK_EQ(id0, 0);
        CHECK_EQ(id1, 1);
        CHECK_EQ(id2, 2);
        CHECK_EQ(id3, 3);

        gen.return_to_poll(id1);
        gen.return_to_poll(id1);

        const auto id4 = gen.fetch();

        CHECK_EQ(id4, id1);

        const auto id5 = gen.fetch();

        CHECK_NE(id5, id1);
        CHECK_EQ(id5, 4);

        gen.return_to_poll(id2);
        gen.return_to_poll(id3);

        const auto id6 = gen.fetch();
        const auto id7 = gen.fetch();

        CHECK_EQ(id6, id3);
        CHECK_EQ(id7, id2);
    }
}
