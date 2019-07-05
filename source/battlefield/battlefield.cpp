#include "battlefield.h"

#include <algorithm>
#include <random>

#include "log.h"
#include "messaging/concrete_message.h"
#include "messaging/messaging.h"
#include "randomizer.h"
#include "unit/unit.h"

void Battlefield::carry_fight(std::shared_ptr<Message_bus>& bus) {
    ENGINE_INFO("Carrying fight.");

    auto msg = std::make_shared<Battle_ended_msg>();

    auto los      = randomizer::uniform_int(0, 1);
    auto loser_it = _bucket.begin();
    if (los)
        ++loser_it;

    msg->loser = loser_it->first;

    msg->retreat_distance = randomizer::uniform_int(0, 5);

    for(const auto& u : loser_it->second){
        msg->retreating_units.insert(u->get_id());
    }

    for (auto& node : _bucket) {
        for (auto& u : node.second) {
            auto r = randomizer::uniform_int(0, u->get_st_points());
            msg->loses.insert({u->get_id(), r});
        }
    }

    bus->queue_message(msg);

    /*
    int pl1_strength = 0;
    for (auto& u : _player1_units)
        pl1_strength += u->get_strength_points();

    int pl2_strength = 0;
    for (auto& u : _player2_units)
        pl2_strength += u->get_strength_points();

    std::uniform_int_distribution<> pl1_distribution(0, pl1_strength / 10);
    std::uniform_int_distribution<> pl2_distribution(0, pl2_strength / 10);

    auto pl2_loss = pl1_distribution(Randomizer::get_engine());
    auto pl1_loss = pl2_distribution(Randomizer::get_engine());

    GAME_INFO("Player1 lost {0} pts.", pl1_loss);
    GAME_INFO("Player2 lost {0} pts.", pl2_loss);
    */
}

void Battlefield::push(Unit* unit, const std::string& owner_name) {
    _bucket[owner_name].insert(unit);
    ENGINE_ASSERT(_bucket.size() < 3, "Battlefield can accept no more than 2 sides of conflict.");
}

void Battlefield::push(std::set<Unit*> units, const std::string& owner_name) {
    _bucket[owner_name].merge(units);
    ENGINE_ASSERT(_bucket.size() < 3, "Battlefield can accept no more than 2 sides of conflict.");
}
