#include "game_state.h"

void Scenario::next_day() { ++_current_day;
    if (auto it = _daily_scripts.find(_current_day); it != _daily_scripts.end()){
        (*it)();
    }
}

int Scenario::current_day() const { return _current_day; }
