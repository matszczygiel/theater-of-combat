#ifndef GRAPHICS_STATE_H
#define GRAPHICS_STATE_H

#include <memory>

#include "toc/graphics/map_gfx.h"
#include "toc/graphics/unit_gfx.h"
#include "toc/gameplay/scenario.h"

class GfxState {
   public:
    explicit GfxState(const std::shared_ptr<Scenario>& sc);

    void update();
    void draw(sf::RenderTarget& target) const;
    void highlight_hex(HexCoordinate coord);

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