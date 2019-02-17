#include "passage_site.h"
#include "log.h"

void Passage_site::set_sides(const Directions &side1, Map_site *site1,
                             const Directions &side2, Map_site *site2) {
    if (side1 != Map_site::opposite_direction(side2)) {
        GAME_FATAL("Passage site initialized with not oposite sites");
    }

if (side1 != Map_site::opposite_direction(side2)) {
        GAME_FATAL("Passage site initialized with not oposite sites");
    }

    _sides           = {site1, site2};
    _sides_direction = {}
}