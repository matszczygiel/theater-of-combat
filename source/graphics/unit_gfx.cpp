#include "unit_gfx.h"

#include "unit/unit_components.h"

UnitGfx::UnitGfx(std::shared_ptr<Layout>& layout) : _layout{layout} {}

void UnitGfx::update(UnitManager& manager) {
    tokens.clear();
    for (const auto& [id, unit] : manager.units()) {
        const auto mov_cmp = manager.get_component<MovementComponent>(id);
        if (!mov_cmp)
            continue;

        const auto pos = mov_cmp->position;
        if (!pos)
            continue;

        tokens.emplace_back(std::make_pair(id, Token(_layout, *pos)));
    }
}

void UnitGfx::draw_tokens(sf::RenderTarget& target) const {
    for (const auto& [id, shape] : tokens) {
        target.draw(shape.shape());
    }
}
