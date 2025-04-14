// Slowdown.h
#ifndef SLOWDOWN_H
#define SLOWDOWN_H
#include "Move.h"
#include "Types.h"
#include "Pokemon.h"
#include "AttackDex.h"

class Slowdown {
    public:
        static void main();
        static void multiPopulate();
        static void damageCalc (Pokemon* attacker, Pokemon* defender, Move* move);
        static std::vector<int> typeMultiplier(int damage, Move* move, Pokemon* defender);
        static void statusCalc(Pokemon* attacker, Pokemon* defender, Move* move);
        static Pokemon battle(Pokemon* friend_pokemon, Pokemon* foe);
};

#endif