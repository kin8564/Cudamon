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
#include <fstream>
#include <random>

#ifndef COMMON_H
#include "common.h"
#endif

const int ITERS = 1;
const int ITERSGPU = 1;

// Prints out the Pokemon and its stats
void displayPoke(int* P) {

}

// Main for parellel pokemon battles
// TODO
int main() {
	std::ofstream nullStream("/dev/null"); // On Windows, use "nul"
	std::streambuf* oldBuf = std::cout.rdbuf();
	std::cout.rdbuf(nullStream.rdbuf());

	// Printing for Output file
	std::ofstream outFile("results.txt", std::ios::app);
	if (!outFile) {
		std::cerr << "Error opening file for writing!" << std::endl;
		return 1;
	}

	outFile << std::scientific;

	clock_t start, end;
	float tcpu, tgpu;
	AttackDex attackdex;
	attackdex.populate();
	PokeDex pokedex;
	pokedex.dexpopulate();

	Pokemon Venusaur = pokedex.pokedex[3];
	Pokemon Charizard = pokedex.pokedex[6];

	Pokemon Charmander = pokedex.pokedex[4];
	Pokemon Squirtle = pokedex.pokedex[7];

	// CPU
	Pokemon* team1 = new Pokemon[MAX_POKEMON];
	Pokemon* team2 = new Pokemon[MAX_POKEMON];
	Pokemon* winner = new Pokemon[MAX_POKEMON];

	for (int i = 0; i < MAX_POKEMON; i++) {
		team1[i] = Charmander;
		team2[i] = Squirtle;
	}

	// Initialize trainer teams
	// Allocation for GPU
	PokemonData* h_p1 = new PokemonData;
	PokemonData* h_p2 = new PokemonData;
	int* h_results = new int[MAX_POKEMON];

	// Initialization
	for (int i = 0; i < MAX_POKEMON; i++) {
		// Player 1 (Charmander)
		h_p1->healthPoints[i] = Charmander.healthPoints;
		h_p1->attack[i] = Charmander.attack;
		h_p1->defense[i] = Charmander.defense;
		h_p1->specialAttack[i] = Charmander.specialAttack;
		h_p1->specialDefense[i] = Charmander.specialDefense;
		h_p1->speed[i] = Charmander.speed;
		h_p1->accuracy[i] = Charmander.pokeacc;
		h_p1->evasion[i] = Charmander.evasion;
		h_p1->type1[i] = Charmander.type1;
		h_p1->type2[i] = Charmander.type2;
		h_p1->nature[i] = Charmander.weirdo;

		for (int j = 0; j < 4; j++) {
			h_p1->moves[i][j] = Charmander.moves[j];
		}

		h_p1->statAtk[i][0] = Charmander.statAtk[0];
		h_p1->statAtk[i][1] = Charmander.statAtk[1];
		h_p1->statDef[i][0] = Charmander.statDef[0];
		h_p1->statDef[i][1] = Charmander.statDef[1];
		h_p1->statSpa[i][0] = Charmander.statSpa[0];
		h_p1->statSpa[i][1] = Charmander.statSpa[1];
		h_p1->statSpd[i][0] = Charmander.statSpd[0];
		h_p1->statSpd[i][1] = Charmander.statSpd[1];
		h_p1->statSpe[i][0] = Charmander.statSpe[0];
		h_p1->statSpe[i][1] = Charmander.statSpe[1];

		h_p1->statAcc[i][0] = Charmander.statAcc[0];
		h_p1->statAcc[i][1] = Charmander.statAcc[1];
		h_p1->statEva[i][0] = Charmander.statEva[0];
		h_p1->statEva[i][1] = Charmander.statEva[1];

		// Player 2 (Squirtle)
		h_p2->healthPoints[i] = Squirtle.healthPoints;
		h_p2->attack[i] = Squirtle.attack;
		h_p2->defense[i] = Squirtle.defense;
		h_p2->specialAttack[i] = Squirtle.specialAttack;
		h_p2->specialDefense[i] = Squirtle.specialDefense;
		h_p2->speed[i] = Squirtle.speed;
		h_p2->accuracy[i] = Squirtle.pokeacc;
		h_p2->evasion[i] = Squirtle.evasion;
		h_p2->type1[i] = Squirtle.type1;
		h_p2->type2[i] = Squirtle.type2;
		h_p2->nature[i] = Squirtle.weirdo;

		for (int j = 0; j < 4; j++) {
			h_p2->moves[i][j] = Squirtle.moves[j];
		}

		h_p2->statAtk[i][0] = Squirtle.statAtk[0];
		h_p2->statAtk[i][1] = Squirtle.statAtk[1];
		h_p2->statDef[i][0] = Squirtle.statDef[0];
		h_p2->statDef[i][1] = Squirtle.statDef[1];
		h_p2->statSpa[i][0] = Squirtle.statSpa[0];
		h_p2->statSpa[i][1] = Squirtle.statSpa[1];
		h_p2->statSpd[i][0] = Squirtle.statSpd[0];
		h_p2->statSpd[i][1] = Squirtle.statSpd[1];
		h_p2->statSpe[i][0] = Squirtle.statSpe[0];
		h_p2->statSpe[i][1] = Squirtle.statSpe[1];

		h_p2->statAcc[i][0] = Squirtle.statAcc[0];
		h_p2->statAcc[i][1] = Squirtle.statAcc[1];
		h_p2->statEva[i][0] = Squirtle.statEva[0];
		h_p2->statEva[i][1] = Squirtle.statEva[1];
	}

	// Run battles in series
	start = clock();
	for (int i = 0; i < ITERS; i++) {
		pokeBattleCPU(team1, team2, winner, MAX_POKEMON);
	}
	end = clock();
	tcpu = (float)(end - start) * 1000 / (float)CLOCKS_PER_SEC / ITERS;
	std::cout.rdbuf(oldBuf);

	printf("Finished CPU");

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<> dist(0, 100);

	int seed = rng() % 100;

	bool success = pokeBattleGPUNew(h_p1, h_p2, h_results, MAX_POKEMON, seed);
	if (!success) {
		std::cout << "\n * Device error! * \n" << std::endl;
		return 1;
	}

	// Run battles in parallel
	start = clock();
	for (int i = 0; i < ITERSGPU; i++) {
		pokeBattleGPUNew(h_p1, h_p2, h_results, MAX_POKEMON, seed);
	}
	end = clock();
	tgpu = (float)(end - start) * 1000 / (float)CLOCKS_PER_SEC / ITERS;

	float speedup = tcpu / tgpu;
	printf("CPU time: %e\n", tcpu);
	printf("GPU time: %e\n", tgpu);
	printf("Speedup: %e\n", speedup);
	// CPU results
	int poke1 = 0;
	for (int i = 0; i < MAX_POKEMON; i++) {
		//printf("Winner CPU %s\n", winner[i].Pokename);
		if (std::strcmp(winner[i].Pokename, "Charmander") == 0) {
			poke1++;
		}
	}
	std::cout << "Charmander Win CPU " << poke1 << std::endl;
	
	// GPU results
	int venuwin = 0;
	for (int i = 0; i < MAX_POKEMON; i++) {
		if (h_results[i] == 1) {
			venuwin++;
		}
	}
	std::cout << "Charmander Win " << venuwin << std::endl;

	//for (int i = 0; i < team_size; ++i) {
	//	std::cout << "Battle " << i + 1 << " result: Winner = Pokémon " << h_results[i] << std::endl;
	//}

	//printf("Done again");
	delete[] team1;
	delete[] team2;
	delete[] winner;
	delete[] h_p1;
	delete[] h_p2;
	delete[] h_results;
	return 0;
} // main