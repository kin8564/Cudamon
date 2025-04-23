#include <iostream>
#include <unordered_map>
#include <string>
#include "Move.h"
#include "Types.h"
#include "AttackDex.h"

std::unordered_map<std::string, Move> attackdex;

void AttackDex::populate() {
    int empty[8] = {0};  // default zeroed effect

    attackdex["Tackle"] = Move("Tackle", TYPES::NORMAL, 1, 40, 100, 35, empty);
    attackdex["Scratch"] = Move("Scratch", TYPES::NORMAL, 1, 40, 100, 35, empty);

    int growlEffect[8] = {1, -1, 0, 0, 0, 0, 0, 0}; // lower target attack
    attackdex["Growl"] = Move("Growl", TYPES::NORMAL, 3, 0, 100, 40, growlEffect);

    int tailWhipEffect[8] = {1, 0, -1, 0, 0, 0, 0, 0}; // lower target defense
    attackdex["Tail Whip"] = Move("Tail Whip", TYPES::NORMAL, 3, 0, 100, 40, tailWhipEffect);

    attackdex["Vine Whip"] = Move("Vine Whip", TYPES::GRASS, 1, 45, 100, 25, empty);
    attackdex["Water Gun"] = Move("Water Gun", TYPES::WATER, 2, 40, 100, 25, empty);
    attackdex["Ember"] = Move("Ember", TYPES::FIRE, 2, 40, 100, 25, empty);

    int growthEffect[8] = {0, 1, 0, 1, 0, 0, 0, 0}; // raise user atk and spa
    attackdex["Growth"] = Move("Growth", TYPES::NORMAL, 3, 0, 0, 20, growthEffect);

    int withdrawEffect[8] = {0, 0, 1, 0, 0, 0, 0, 0}; // raise user defense
    attackdex["Withdraw"] = Move("Withdraw", TYPES::WATER, 3, 0, 0, 40, withdrawEffect);

    int smokescreenEffect[8] = {1, 0, 0, 0, 0, 0, -1, 0}; // lower target accuracy
    attackdex["Smokescreen"] = Move("Smokescreen", TYPES::NORMAL, 3, 0, 100, 20, smokescreenEffect);
}
