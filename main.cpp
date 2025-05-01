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

	// Initialize trainer teams
	// Allocation for GPU
	PokemonData* h_p1 = new PokemonData;
	PokemonData* h_p2 = new PokemonData;
	int* h_results = new int[team_size];

	Pokemon Venusaur = pokedex.pokedex[3];
	Pokemon Charizard = pokedex.pokedex[6];

	// Initialization
	for (int i = 0; i < team_size; i++) {
		// Player 1 (Venusaur)
		h_p1->healthPoints[i] = Venusaur.healthPoints;
		h_p1->attack[i] = Venusaur.attack;
		h_p1->defense[i] = Venusaur.defense;
		h_p1->specialAttack[i] = Venusaur.specialAttack;
		h_p1->specialDefense[i] = Venusaur.specialDefense;
		h_p1->speed[i] = Venusaur.speed;
		h_p1->accuracy[i] = Venusaur.pokeacc;
		h_p1->evasion[i] = Venusaur.evasion;
		h_p1->type1[i] = Venusaur.type1;
		h_p1->type2[i] = Venusaur.type2;
		h_p1->nature[i] = Venusaur.weirdo;

		for (int j = 0; j < 4; j++) {
			h_p1->moves[i][j] = Venusaur.moves[j];
		}

		h_p1->statAtk[i][0] = Venusaur.statAtk[0];
		h_p1->statAtk[i][1] = Venusaur.statAtk[1];
		h_p1->statDef[i][0] = Venusaur.statDef[0];
		h_p1->statDef[i][1] = Venusaur.statDef[1];
		h_p1->statSpa[i][0] = Venusaur.statSpa[0];
		h_p1->statSpa[i][1] = Venusaur.statSpa[1];
		h_p1->statSpd[i][0] = Venusaur.statSpd[0];
		h_p1->statSpd[i][1] = Venusaur.statSpd[1];
		h_p1->statSpe[i][0] = Venusaur.statSpe[0];
		h_p1->statSpe[i][1] = Venusaur.statSpe[1];

		h_p1->statAcc[i][0] = Venusaur.statAcc[0];
		h_p1->statAcc[i][1] = Venusaur.statAcc[1];
		h_p1->statEva[i][0] = Venusaur.statEva[0];
		h_p1->statEva[i][1] = Venusaur.statEva[1];

		// Player 2 (Charizard)
		h_p2->healthPoints[i] = Charizard.healthPoints;
		h_p2->attack[i] = Charizard.attack;
		h_p2->defense[i] = Charizard.defense;
		h_p2->specialAttack[i] = Charizard.specialAttack;
		h_p2->specialDefense[i] = Charizard.specialDefense;
		h_p2->speed[i] = Charizard.speed;
		h_p2->accuracy[i] = Charizard.pokeacc;
		h_p2->evasion[i] = Charizard.evasion;
		h_p2->type1[i] = Charizard.type1;
		h_p2->type2[i] = Charizard.type2;
		h_p2->nature[i] = Charizard.weirdo;

		for (int j = 0; j < 4; j++) {
			h_p2->moves[i][j] = Charizard.moves[j];
		}

		h_p2->statAtk[i][0] = Charizard.statAtk[0];
		h_p2->statAtk[i][1] = Charizard.statAtk[1];
		h_p2->statDef[i][0] = Charizard.statDef[0];
		h_p2->statDef[i][1] = Charizard.statDef[1];
		h_p2->statSpa[i][0] = Charizard.statSpa[0];
		h_p2->statSpa[i][1] = Charizard.statSpa[1];
		h_p2->statSpd[i][0] = Charizard.statSpd[0];
		h_p2->statSpd[i][1] = Charizard.statSpd[1];
		h_p2->statSpe[i][0] = Charizard.statSpe[0];
		h_p2->statSpe[i][1] = Charizard.statSpe[1];

		h_p2->statAcc[i][0] = Charizard.statAcc[0];
		h_p2->statAcc[i][1] = Charizard.statAcc[1];
		h_p2->statEva[i][0] = Charizard.statEva[0];
		h_p2->statEva[i][1] = Charizard.statEva[1];
	}

	// Run battles in series
	for (int i = 0; i < ITERS; i++) {
		Pokemon victor;
		pokeBattleCPU(Charizard, Venusaur, victor);
	}

	//bool success = pokeBattleGPUNew(h_p1, h_p2, h_results, team_size);
	//if (!success) {
	//	std::cout << "\n * Device error! * \n" << std::endl;
	//	return 1;
	//}
	//printf("Done 1k");
	//// Run battles in parallel
	//for (int i = 0; i < ITERSGPU; i++) {
	//	pokeBattleGPUNew(h_p1, h_p2, h_results, team_size);
	//}

	//int squirtwin = 0;
	//for (int i = 0; i < team_size; i++) {
	//	if (h_results[i] == 2) {
	//		squirtwin++;
	//	}
	//}
	//std::cout << "Charizard Win " << squirtwin << std::endl;

	//for (int i = 0; i < team_size; ++i) {
	//	std::cout << "Battle " << i + 1 << " result: Winner = Pokémon " << h_results[i] << std::endl;
	//}

	//printf("Done again");
	delete[] h_p1;
	delete[] h_p2;
	delete[] h_results;
	return 0;
} // main