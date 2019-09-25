#ifndef GRAPHICS_STATE_H
#define GRAPHICS_STATE_H

#include <memory>

#include "map_gfx.h"
#include "unit_gfx.h"

#include "gameplay/game_state.h"

class GfxState {
   public:
    explicit GfxState(const GameState& state);

    void update();
    void draw(sf::RenderTarget& target) const;

    std::shared_ptr<Layout> layout{std::make_shared<Layout>()};

    MapGfx map{layout};
    UnitGfx units{layout};

    bool debug_coords{true};
    bool debug_units{true};

    std::vector<sf::ConvexShape> highlighted_hexes{};

    sf::Font font;

   private:
    const std::shared_ptr<Scenario> _scenario{nullptr};
};

#endif