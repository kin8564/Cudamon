#include <math.h>
#include <iostream> // cout, stream

#ifndef COMMON_H
#include "common.h"
#endif

/**
 * SUBJECT TO CHANGE
 * Computes the CPU Pokemon battle algorithm
 * pokemon1 - Input array of pokemon1 stats
 * pokemon2 - Input array of pokemon2 stats
 * winner - Output array of winning pokemon
 * loser - Output array of losing pokemon
 * battleResult - Output array of battle results
 * numBattles - Number of battles to run
 */
Pokemon pokeBattleCPU(Pokemon& pokemon1, Pokemon& pokemon2) {
    // Integrate process from Slowdown
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> dist(0, 100);

    // Until KO
    while (pokemon1.getHP() >= 1 && pokemon2.getHP() >= 1) {
        Move selected;
        if (pokemon1.getSpe() > pokemon2.getSpe()) {
            // Friend attacks first
            selected = pokemon1.getMove(rng() % pokemon1.getMovesNum());

            if (selected.getCat() == 3) {
                statusCalc(pokemon1, pokemon2, selected);
            }
            else {
                damageCalc(pokemon1, pokemon2, selected);
            }

            if (pokemon2.getHP() < 1) break;

            // Foe attacks
            selected = pokemon2.getMove(rng() % pokemon2.getMovesNum());

            if (selected.getCat() == 3) {
                statusCalc(pokemon2, pokemon1, selected);
            }
            else {
                damageCalc(pokemon2, pokemon1, selected);
            }

        }
        else {
            // Foe attacks first
            selected = pokemon2.getMove(rng() % pokemon2.getMovesNum());

            if (selected.getCat() == 3) {
                statusCalc(pokemon2, pokemon1, selected);
            }
            else {
                damageCalc(pokemon2, pokemon1, selected);
            }

            if (pokemon1.getHP() < 1) break;

            // Friend attacks
            selected = pokemon1.getMove(rng() % pokemon1.getMovesNum());

            if (selected.getCat() == 3) {
                statusCalc(pokemon1, pokemon2, selected);
            }
            else {
                damageCalc(pokemon1, pokemon2, selected);
            }
        }
    }

    if (pokemon2.getHP() < 1) {
        return pokemon1;
    }
    else {
        return pokemon2;
    }

} // pokeBattleCPU