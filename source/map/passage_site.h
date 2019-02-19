#pragma once

#include "hex_site.h"
#include "map_site.h"

class Passage_site : public Map_site {
   public:
    explicit Passage_site(const int &number = 0);

    virtual ~Passage_site() noexcept = default;

    virtual void draw(sf::RenderTarget &target) const override;
    virtual void set_highlighted(bool highlighted) noexcept override;
    void set_sides(const Directions &side1, Hex_site *site1,
                   const Directions &side2, Hex_site *site2);
    void unstage_sides();
    Hex_site *other_side(Hex_site *adjacet_site);

   protected:
    virtual void set_shape() = 0;

    sf::RectangleShape _shape;
    std::pair<Hex_site *, Hex_site *> _sides;

   private:
    std::pair<Directions, Directions> _sides_directions;
};