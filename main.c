#include <cstdlib> // malloc(), free()
#include <ctime> // time(), clock()
#include <cmath> // sqrt()
#include <iostream> // cout, stream
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>

#ifndef COMMON_H
#include "common.h"
#endif

const int ITERS = 1;

// Prints out the Pokemon and its stats
void displayPoke(int* P) {

}

// Main for parellel pokemon battles
// TODO
int main() {

	// Battle Results
	float wins, losses;

	// Initialize trainer teams
	// Does this require memory allocation? Maybe arrays of ints as long as the necessary state

	// Display teams
		// Print pokemon names, levels, moves, etc.

	// Run battles in series
	for (int i = 0; i < ITERS; i++) {
		pokeBattleCPU(0, 0, 0, 0, 0);
	}

	// Display results
		// Print win / loss ratio, most successful build, etc.

	// Run battles in parallel
	for (int i = 0; i < ITERS; i++) {
		//pokeBattleGPU(pokemon1, pokemon2, winner, loser, battleResult);
	}

	// Success
	return 0;
} // main