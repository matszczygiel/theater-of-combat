#pragma once

#include "map_site.h"

#include <array>

class Passage_site : public Map_site {
   public:
    explicit Passage_site(const int &number = 0) {
        _highlighted = false;
        _number      = number;
        _sides       = {nullptr, nullptr};
    }

    virtual ~Passage_site() = default;

    void set_sides(const Directions &side1, Map_site *site1,
                   const Directions &side2, Map_site *site2);

   protected:
    void set_side_this(const Directions &side, Map_site *site) override;
    virtual void set_color() = 0;

   private:
    std::array<Map_site *, 2> _sides;
    std::array<Directions, 2> _sides_direction;
}