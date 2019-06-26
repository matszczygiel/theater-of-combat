#include "team.h"

#include "map/hex_site.h"
#include "map/map.h"
#include "unit/unit.h"
#include "unit/unit_set.h"

bool Team::remove_unit_ownership(const int& no) {
    return _owned_units_ids.erase(no) == 1;
}

bool Team::add_unit_ownership(const int& no) {
    return _owned_units_ids.insert(no).second;
}

std::vector<int> Team::get_controlable_hexes() const {
    std::vector<int> vec;

    for (auto& u : _owned_units_ids) {
        const auto occ = _unit_set->get_by_id(u)->get_occupation();
        if (!occ)
            continue;
        auto tmp_vec = _map->get_controlable_hexes_from(occ->get_number());
        vec.insert(vec.end(), tmp_vec.begin(), tmp_vec.end());
    }

    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
    return vec;
}

void Team::draw(sf::RenderTarget& target) const {
    for (auto& u : _units_to_draw)
        u->draw(target);
}