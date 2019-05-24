#pragma once

#include <memory>

#include "hex_site.h"
#include "map_site.h"

#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/utility.hpp>

enum class Passage_type {
    stream,
    creek,
    river,
    huge_river,
    road
};

std::string type_to_string(const Passage_type &type);
Passage_type string_to_passage_type(const std::string &str);

class Passage_site : public Map_site, public std::enable_shared_from_this<Passage_site> {
   public:
    explicit Passage_site(const int &number = 0);

    virtual ~Passage_site() noexcept = default;

    virtual void set_highlighted(bool highlighted) noexcept override;
    virtual Passage_type get_passage_type() const = 0;

    void set_sides(const Directions &side1, std::shared_ptr<Hex_site> site1,
                   const Directions &side2, std::shared_ptr<Hex_site> site2);
    void unstage_sides();
    Site_type get_site_type() const final;
    std::shared_ptr<Hex_site> other_side(std::shared_ptr<Hex_site> adjacent_site);
    const auto &get_sides() const;
    const auto &get_sides_directions() const;

    void write(pugi::xml_node &node);

   protected:
    virtual void set_shape() = 0;

    std::pair<std::shared_ptr<Hex_site>, std::shared_ptr<Hex_site> > _sides;

   private:
    std::pair<Directions, Directions> _sides_directions;

   public:
    template <class Archive>
    void serialize(Archive &ar) {
        ar(cereal::virtual_base_class<Map_site>(this),
           CEREAL_NVP(_sides), CEREAL_NVP(_sides_directions));
    }
    /*
    template <class Archive>
    friend void serialize(Archive &ar, Passage_site &p);*/
};
/*
template <class Archive>
void serialize(Archive &ar, Passage_site &p) {
    ar(cereal::virtual_base_class<Map_site>(&p), CEREAL_NVP(p._sides), CEREAL_NVP(p._sides_directions));
}
*/
inline Site_type Passage_site::get_site_type() const {
    return Site_type::passage;
}

inline const auto &Passage_site::get_sides() const {
    return _sides;
}

inline const auto &Passage_site::get_sides_directions() const {
    return _sides_directions;
}