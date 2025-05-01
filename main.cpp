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
#include "PokemonData.h"

#ifndef COMMON_H
#include "common.h"
#endif

const int ITERS = 1;
const int ITERSGPU = 1;
const int team_size = 1000;

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

	//// Initialize trainer teams
	//// Does this require memory allocation? Maybe arrays of ints as long as the necessary state
	//Pokemon* team1 = new Pokemon[team_size];  // team1 is now dynamically allocated
	//Pokemon* team2 = new Pokemon[team_size];  // team2 is now dynamically allocated

	//Pokemon resultsCPU[ITERS];

	//Pokemon* resultsGPU = new Pokemon[team_size];
	//// All Charmander
	//for (int i = 0; i < team_size; i++) {
	//	team1[i] = pokedex.pokedex[4];
	//	team2[i] = pokedex.pokedex[7];
	//}

	////Pokemon Charmander = pokedex.pokedex[4];
	////Pokemon Squirtle = pokedex.pokedex[7];

	////// Display teams
	////	// Print pokemon names, levels, moves, etc.

	////// Run battles in series
	////for (int i = 0; i < ITERS; i++) {
	////	Pokemon victor;
	////	pokeBattleCPU(Charmander, Squirtle, victor);
	////}
	////printf("Done\n");
	//// Display results
	//	// Print win / loss ratio, most successful build, etc.

	//bool success = pokeBattleGPU(team1, team2, resultsGPU, team_size);
	//if (!success) {
	//	std::cout << "\n * Device error! * \n" << std::endl;
	//	return 1;
	//}
	//printf("Done first 1k");

	//// Run battles in parallel
	//for (int i = 0; i < ITERSGPU; i++) {
	//	pokeBattleGPU(team1, team2, resultsGPU, team_size);
	//}

	//for (int i = 0; i < team_size; i++) {
	//	printf("Winner: %s", resultsGPU[i].Pokename);
	//}
	//delete[] team1;
	//delete[] team2;
	//delete[] resultsGPU;
	// Success
	//###############################################################################################################
	PokemonData* h_p1 = new PokemonData[team_size];
	PokemonData* h_p2 = new PokemonData[team_size];
	int* h_results = new int[team_size];

	Pokemon Charmander = pokedex.pokedex[4];
	Pokemon Squirtle = pokedex.pokedex[7];
	// Initialization
	for (int i = 0; i < team_size; i++) {
		h_p1[i].healthPoints[0] = Charmander.healthPoints;
		h_p1[i].attack[0] = Charmander.attack;
		h_p1[i].defense[0] = Charmander.defense;
		h_p1[i].specialAttack[0] = Charmander.specialAttack;
		h_p1[i].specialDefense[0] = Charmander.specialDefense;
		h_p1[i].speed[0] = Charmander.speed;
		h_p1[i].accuracy[0] = Charmander.pokeacc;
		h_p1[i].evasion[0] = Charmander.evasion;
		h_p1[i].type1[0] = Charmander.type1;
		h_p1[i].type2[0] = Charmander.type2;
		h_p1[i].moves[0][0] = Charmander.moves[0];
		h_p1[i].moves[0][1] = Charmander.moves[1];
		h_p1[i].moves[0][2] = Charmander.moves[2];
		h_p1[i].moves[0][3] = Charmander.moves[3];

		h_p2[i].healthPoints[0] = Squirtle.healthPoints;
		h_p2[i].attack[0] = Squirtle.attack;
		h_p2[i].defense[0] = Squirtle.defense;
		h_p2[i].specialAttack[0] = Squirtle.specialAttack;
		h_p2[i].specialDefense[0] = Squirtle.specialDefense;
		h_p2[i].speed[0] = Squirtle.speed;
		h_p2[i].accuracy[0] = Squirtle.pokeacc;
		h_p2[i].evasion[0] = Squirtle.evasion;
		h_p2[i].type1[0] = Squirtle.type1;
		h_p2[i].type2[0] = Squirtle.type2;
		h_p2[i].moves[0][0] = Squirtle.moves[0];
		h_p2[i].moves[0][1] = Squirtle.moves[1];
		h_p2[i].moves[0][2] = Squirtle.moves[2];
		h_p2[i].moves[0][3] = Squirtle.moves[3];
	}

	bool success = pokeBattleGPUNew(h_p1, h_p2, h_results, team_size);
	if (!success) {
		std::cout << "\n * Device error! * \n" << std::endl;
		return 1;
	}
	printf("Done first 1k");

	// Run battles in parallel
	for (int i = 0; i < ITERSGPU; i++) {
		pokeBattleGPUNew(h_p1, h_p2, h_results, team_size);
	}

	printf("Done again");
	delete[] h_p1;
	delete[] h_p2;
	delete[] h_results;
	return 0;
} // main