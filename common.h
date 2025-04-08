/************************************************************************/
/* Pokemon Battle                                                       */
/* common.h                                                             */
/* This file contains the function prototypes that will be called by    */
/* the application developed.                                           */
/*                                                                      */
/* Kingsley Nwabeke, Mikyle Gregory                                     */
/* April 8, 2025                                                        */
/************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

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
void pokeBattleCPU( int* pokemon1, int* pokemon2, int* winner, int* loser, int* battleResult );

/**
 * SUBJECT TO CHANGE
 * Computes the GPU Pokemon battle algorithm
 * pokemon1 - Input array of pokemon1 stats
 * pokemon2 - Input array of pokemon2 stats
 * winner - Output array of winning pokemon
 * loser - Output array of losing pokemon
 * battleResult - Output array of battle results
 * numBattles - Number of battles to run
 */
bool pokeBattleGPU( int* pokemon1, int* pokemon2, int* winner, int* loser, int* battleResult );

#endif