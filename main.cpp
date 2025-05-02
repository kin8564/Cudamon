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
#include "Natures.h"
#include "PokemonData.h"
#include <fstream>
#include <random>

#ifndef COMMON_H
#include "common.h"
#endif

const int ITERS = 1;
const int ITERSGPU = 1;
const int team_size = 1000;

// Prints out the Pokemon and its stats
void displayPoke(int* P) {

}

//Modulo of Idx is dependent on the team size variable, consider passing that in to function
void natureSet(int idx, Pokemon mon) {
	int choose = idx % 25;
	switch (choose) {
	case 0: mon.setNature(NATURES::HARDY);
	case 1: mon.setNature(NATURES::LONELY);
	case 2: mon.setNature(NATURES::BRAVE);
	case 3: mon.setNature(NATURES::ADAMANT);
	case 4: mon.setNature(NATURES::NAUGHTY);
	case 5: mon.setNature(NATURES::BOLD);
	case 6: mon.setNature(NATURES::DOCILE);
	case 7: mon.setNature(NATURES::RELAXED);
	case 8: mon.setNature(NATURES::IMPISH);
	case 9: mon.setNature(NATURES::LAX);
	case 10: mon.setNature(NATURES::TIMID);
	case 11: mon.setNature(NATURES::HASTY);
	case 12: mon.setNature(NATURES::SERIOUS);
	case 13: mon.setNature(NATURES::JOLLY);
	case 14: mon.setNature(NATURES::NAIVE);
	case 15: mon.setNature(NATURES::MODEST);
	case 16: mon.setNature(NATURES::MILD);
	case 17: mon.setNature(NATURES::QUIET);
	case 18: mon.setNature(NATURES::BASHFUL);
	case 19: mon.setNature(NATURES::RASH);
	case 20: mon.setNature(NATURES::CALM);
	case 21: mon.setNature(NATURES::GENTLE);
	case 22: mon.setNature(NATURES::SASSY);
	case 23: mon.setNature(NATURES::CAREFUL);
	case 24: mon.setNature(NATURES::QUIRKY);
	}
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
		team1[i] = Charizard;
		team2[i] = Venusaur;
	}

	// Initialize trainer teams
	// Allocation for GPU
	PokemonData* h_p1 = new PokemonData;
	PokemonData* h_p2 = new PokemonData;
	int* h_results = new int[MAX_POKEMON];
	int* turn_results = new int[MAX_POKEMON];

	// Initialization
	for (int i = 0; i < team_size; i++) {
		// Player 1 (Charmander)
		h_p1->healthPoints[i] = Charizard.healthPoints;
		h_p1->attack[i] = Charizard.attack;
		h_p1->defense[i] = Charizard.defense;
		h_p1->specialAttack[i] = Charizard.specialAttack;
		h_p1->specialDefense[i] = Charizard.specialDefense;
		h_p1->speed[i] = Charizard.speed;
		h_p1->accuracy[i] = Charizard.pokeacc;
		h_p1->evasion[i] = Charizard.evasion;
		h_p1->type1[i] = Charizard.type1;
		h_p1->type2[i] = Charizard.type2;
		natureSet(i, Charizard);
		h_p1->nature[i] = Charizard.weirdo;

		for (int j = 0; j < 4; j++) {
			h_p1->moves[i][j] = Charizard.moves[j];
		}

		h_p1->statAtk[i][0] = Charizard.statAtk[0];
		h_p1->statAtk[i][1] = Charizard.statAtk[1];
		h_p1->statDef[i][0] = Charizard.statDef[0];
		h_p1->statDef[i][1] = Charizard.statDef[1];
		h_p1->statSpa[i][0] = Charizard.statSpa[0];
		h_p1->statSpa[i][1] = Charizard.statSpa[1];
		h_p1->statSpd[i][0] = Charizard.statSpd[0];
		h_p1->statSpd[i][1] = Charizard.statSpd[1];
		h_p1->statSpe[i][0] = Charizard.statSpe[0];
		h_p1->statSpe[i][1] = Charizard.statSpe[1];

		h_p1->statAcc[i][0] = Charizard.statAcc[0];
		h_p1->statAcc[i][1] = Charizard.statAcc[1];
		h_p1->statEva[i][0] = Charizard.statEva[0];
		h_p1->statEva[i][1] = Charizard.statEva[1];

		// Player 2 (Squirtle)
		h_p2->healthPoints[i] = Venusaur.healthPoints;
		h_p2->attack[i] = Venusaur.attack;
		h_p2->defense[i] = Venusaur.defense;
		h_p2->specialAttack[i] = Venusaur.specialAttack;
		h_p2->specialDefense[i] = Venusaur.specialDefense;
		h_p2->speed[i] = Venusaur.speed;
		h_p2->accuracy[i] = Venusaur.pokeacc;
		h_p2->evasion[i] = Venusaur.evasion;
		h_p2->type1[i] = Venusaur.type1;
		h_p2->type2[i] = Venusaur.type2;
		h_p2->nature[i] = Venusaur.weirdo;

		for (int j = 0; j < 4; j++) {
			h_p2->moves[i][j] = Venusaur.moves[j];
		}

		h_p2->statAtk[i][0] = Venusaur.statAtk[0];
		h_p2->statAtk[i][1] = Venusaur.statAtk[1];
		h_p2->statDef[i][0] = Venusaur.statDef[0];
		h_p2->statDef[i][1] = Venusaur.statDef[1];
		h_p2->statSpa[i][0] = Venusaur.statSpa[0];
		h_p2->statSpa[i][1] = Venusaur.statSpa[1];
		h_p2->statSpd[i][0] = Venusaur.statSpd[0];
		h_p2->statSpd[i][1] = Venusaur.statSpd[1];
		h_p2->statSpe[i][0] = Venusaur.statSpe[0];
		h_p2->statSpe[i][1] = Venusaur.statSpe[1];

		h_p2->statAcc[i][0] = Venusaur.statAcc[0];
		h_p2->statAcc[i][1] = Venusaur.statAcc[1];
		h_p2->statEva[i][0] = Venusaur.statEva[0];
		h_p2->statEva[i][1] = Venusaur.statEva[1];
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

	bool success = pokeBattleGPUNew(h_p1, h_p2, h_results, turn_results, MAX_POKEMON, seed);
	if (!success) {
		std::cout << "\n * Device error! * \n" << std::endl;
		return 1;
	}

	// Run battles in parallel
	start = clock();
	for (int i = 0; i < ITERSGPU; i++) {
		pokeBattleGPUNew(h_p1, h_p2, h_results, turn_results, MAX_POKEMON, seed);
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
		if (std::strcmp(winner[i].Pokename, "Venusaur") == 0) {
			poke1++;
		}
	}
	std::cout << "Venusaur Win CPU " << poke1 << std::endl;
	
	// GPU results
	int userWin = 0;
	for (int i = 0; i < MAX_POKEMON; i++) {
		if (h_results[i] == 1) {
			userWin++;
		}
	}
	std::cout << "Charizard Win " << userWin << std::endl;

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