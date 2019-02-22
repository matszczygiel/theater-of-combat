#pragma once

#include "hex_site.h"
#include "map_site.h"

enum class Passage_type {
    stream,
    creek,
    river,
    huge_river,
    road
};

class Passage_site : public Map_site {
   public:
    explicit Passage_site(const int &number = 0);

    virtual ~Passage_site() noexcept = default;

    virtual void set_highlighted(bool highlighted) noexcept override;
    virtual Passage_type get_passage_type() const = 0;

    void set_sides(const Directions &side1, Hex_site *site1,
                   const Directions &side2, Hex_site *site2);
    void unstage_sides();
    Site_type get_site_type() const final;
    Hex_site *other_side(Hex_site *adjacet_site);
    const auto &get_sides() const;
    const auto &get_sides_directions() const;

   protected:
    virtual void set_shape() = 0;

    std::pair<Hex_site *, Hex_site *> _sides;

   private:
    std::pair<Directions, Directions> _sides_directions;
};

inline Site_type Passage_site::get_site_type() const {
    return Site_type::passage;
}

inline const auto &Passage_site::get_sides() const {
    return _sides;
}

inline const auto &Passage_site::get_sides_directions() const {
    return _sides_directions;
}