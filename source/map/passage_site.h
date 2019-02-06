#pragma once

#include "map_site.h"

#include <array>

class Passage_site : public Map_site {
    public:
    explicit Passage_site(const int& number = 0) {

    }


    private:
        std::array<Map_site*, 2> _sides;
        
}