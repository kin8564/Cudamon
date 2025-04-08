#include <iostream>
#include <unordered_map>
#include <string>
#include  "Move.h"
#include "Types.h"

class AttackDex{
    public:
        std::unordered_map<std::string, Move*> attackdex;

        void populate(){
            attackdex["Tackle"] = new Move("Tackle", Types::NORMAL, 1, 40, 100, 35, {});
            attackdex["Scratch"] = new Move("Scratch", Types::NORMAL, 1, 40, 100, 35, {});

            std::array status = {1, -1, 0, 0, 0, 0, 0, 0}; // lower target attack by one stage
            attackdex["Growl"] = new Move("Growl", Types::NORMAL, 3, 0, 100, 40, status);

            status = {1, 0, -1, 0, 0, 0, 0, 0};
            attackdex["Tail Whip"] = new Move("Tail Whip", Types::NORMAL, 3, 0, 100, 40, status);
            attackdex["Vine Whip"] = new Move("Vine Whip", Types::GRASS, 1, 45, 100, 25, {});
            attackdex["Water Gun"] = new Move("Water Gun", Types::WATER, 2, 40, 100, 25, {});
            attackdex["Ember"] = new Move("Ember", Types::FIRE, 2, 40, 100, 25, {});

            status = {0, 1, 0, 1, 0, 0, 0, 0}; // raise user atk and spa by one stage
            attackdex["Growth"] = new Move("Growth", Types::NORMAL, 3, 0, 0, 20, status);

            status = {0, 0, 1, 0, 0, 0, 0, 0}; // raise user def one stage
            attackdex["Withdraw"] = new Move("Withdraw", Types::WATER, 3, 0, 0, 40, status);
            
            status = {1, 0, 0, 0, 0, 0, -1, 0}; // lower target acc one stage
            attackdex["Smokescreen"] = new Move("Smokescreen", Types::NORMAL, 3, 0, 100, 20, status);
        }

};