#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H

#include <string>

#include "toc/map/map.h"
#include "toc/gameplay/system_state.h"

class MapCreator {
   public:
    MapCreator(const std::string name) noexcept;

    bool retrieve(std::unique_ptr<SystemState>& system);

    void show(bool* open);
    bool is_ready() const;

   private:
    bool _ready{false};
    int _width{};
    int _height{};
    bool _create_test{false};

    std::string _name{};

};

#endif /* MAP_CREATOR_H */
