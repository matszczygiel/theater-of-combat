#include "team.h"

#include "map/hex_site.h"
#include "unit/unit.h"

void Team::add_unit(std::unique_ptr<Unit> unit) {
    _units.emplace(std::make_pair(unit->get_id(), std::move( unit)));
}

void Team::remove_unit(const int& no) {
    _units.erase(no);
}

std::vector<int> Team::get_controlable_hexes(const Map& map) const {
    std::vector<int> vec;

    for (auto& u : _units) {
        const auto occ = u.second->get_occupation();
        if (!occ)
            continue;
        auto tmp_vec = map.get_controlable_hexes_from(occ->get_number());
        vec.insert(vec.end(), tmp_vec.begin(), tmp_vec.end());
    }

    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
    return vec;
}

void Team::draw(sf::RenderTarget& target) const {
    for(auto& u : _units_to_draw)
        u->draw(target);
}