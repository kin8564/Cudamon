/************************************************************************/
/* Pokemon Battle                                                       */
/* common.h                                                             */
/* This file contains the function prototypes that will be called by    */
/* the application developed.                                           */
/*                                                                      */
/* Kingsley Nwabeke, Mikyle Gregory                                     */
/* April 8, 2025                                                        */
/************************************************************************/
#include "Pokemon.h"
#include "PokemonData.h"

#ifndef __COMMON_H__
#define __COMMON_H__

/**
 * SUBJECT TO CHANGE
 * Computes the CPU Pokemon battle algorithm
 * pokemon1 - Input array of pokemon1 stats
 * pokemon2 - Input array of pokemon2 stats
 * winner - Output of winning pokemon
 */
void pokeBattleCPU(Pokemon* poketeam1, Pokemon* poketeam2, Pokemon* winner, int teamsize);

/**
 * SUBJECT TO CHANGE
 * Computes the GPU Pokemon battle algorithm
 * pokemon1 - Input array of pokemon1 stats
 * pokemon2 - Input array of pokemon2 stats
 * winner - Output of winning pokemon
 */
bool pokeBattleGPU(Pokemon* pokemon1, Pokemon* pokemon2, Pokemon* results, int NUM_POKEMON);

bool pokeBattleGPUNew(PokemonData* p1, PokemonData* p2, int* result, int numBattles, int seed);

#endif