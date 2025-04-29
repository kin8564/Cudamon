#include <cstdlib> // malloc(), free()
#include <ctime> // time(), clock()
#include <cmath> // sqrt()
#include <iostream> // cout, stream
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include "Pokemon.h"
#include "PokeDex.h"
#include "AttackDex.h"

#ifndef COMMON_H
#include "common.h"
#endif

const int ITERS = 10;
const int ITERSGPU = 10;
const int team_size = 6;

// Prints out the Pokemon and its stats
void displayPoke(int* P) {

}

// Main for parellel pokemon battles
// TODO
int main() {
	AttackDex attackdex;
	attackdex.populate();
	PokeDex pokedex;
	pokedex.dexpopulate();

	// Battle Results
	float wins, losses;

	// Initialize trainer teams
	// Does this require memory allocation? Maybe arrays of ints as long as the necessary state
	Pokemon team1[team_size];
	Pokemon team2[team_size];

	Pokemon resultsCPU[ITERS];

	Pokemon resultsGPU[ITERS];
	// All Charmander
	for (int i = 0; i < team_size; i++) {
		team1[i] = pokedex.pokedex[4];
	}

	// All Squirtle
	for (int i = 0; i < team_size; i++) {
		team2[i] = pokedex.pokedex[7];
		//3 columns by moveset, 25 natures
	}

	Pokemon Charmander = pokedex.pokedex[4];
	Pokemon Squirtle = pokedex.pokedex[7];

	// Display teams
		// Print pokemon names, levels, moves, etc.

	// Run battles in series
	for (int i = 0; i < ITERS; i++) {
		Pokemon victor;
		pokeBattleCPU(&Charmander, &Squirtle, &victor);
	}

	// Display results
		// Print win / loss ratio, most successful build, etc.

	bool success = pokeBattleGPU(team1, team2, resultsGPU, team_size);
	if (!success) {
		std::cout << "\n * Device error! * \n" << std::endl;
		return 1;
	}

	// Run battles in parallel
	for (int i = 0; i < ITERSGPU; i++) {
		pokeBattleGPU(team1, team2, resultsGPU, team_size);
	}

	for (int i = 0; i < team_size; i++) {
		printf("Winner: %s", resultsGPU[i]);
	}
	// Success
	return 0;
} // main