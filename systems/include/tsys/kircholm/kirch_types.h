#ifndef KIRCH_TYPES_H
#define KIRCH_TYPES_H

namespace kirch {
enum class HexType {
    field,
    forest,
    swamp,
    hillside_0,
    hillside_1,
    hillside_2,
    hillside_3,
    hillside_4,
    hillside_5,
    hills
};

enum class UnitType { infrantry, cavalary, dragoons, artillery };

enum class FightingPerformance { level_1, level_2 };
}  // namespace kirch

#endif