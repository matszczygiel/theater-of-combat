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

std::set<int> Team::get_controlable_hexes() const {
    std::set<int> set;

    for (auto& u : _owned_units_ids) {
        const auto occ = _unit_set->get_by_id(u)->get_occupation();
        if (!occ)
            continue;
        auto tmp_vec = _map->get_controlable_hexes_from(occ->get_number());
        set.merge(tmp_vec);
    }

    return set;
}

void Team::draw(sf::RenderTarget& target) const {
    for (auto& u : _units_to_draw)
        u->draw(target);
}

void Team::update() {
    _units_to_draw.clear();
    _units_to_draw.reserve(_owned_units_ids.size());
    for (auto& uid : _owned_units_ids) {
        _units_to_draw.push_back(_unit_set->get_by_id(uid));
    }
}

void Team::apply(const std::function<void(Unit&)>& function) {
    _unit_set->apply(_owned_units_ids, function);
}
