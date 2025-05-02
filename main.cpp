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
#include <iomanip>  

#ifndef COMMON_H
#include "common.h"
#endif

const int ITERS = 1;
const int ITERSGPU = 1;

// Prints out the Pokemon and its stats
void displayPoke(int* P) {

}

void natureSet(int idx, Pokemon mon) {
    int groupSize = MAX_POKEMON / 25;
    int group = idx / groupSize;
    switch (group) {
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
	//std::ofstream nullStream("/dev/null"); // On Windows, use "nul"
	//std::streambuf* oldBuf = std::cout.rdbuf();
	//std::cout.rdbuf(nullStream.rdbuf());

	// Printing for Output file
	//std::ofstream outFile("C:/Users/mag6814/Source/Repos/kin8564/Cudamon/results.txt", std::ios::app);
	std::fstream outFile("E:/Cudamon/results.txt", std::ios::app);
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
	Pokemon Blastoise = pokedex.pokedex[9];

	Pokemon Charmander = pokedex.pokedex[4];
	Pokemon Squirtle = pokedex.pokedex[7];

	// CPU
	Pokemon* team1 = new Pokemon[MAX_POKEMON];
	Pokemon* team2 = new Pokemon[MAX_POKEMON];
	Pokemon* winner = new Pokemon[MAX_POKEMON];

	for (int i = 0; i < MAX_POKEMON; i++) {
		team1[i] = Charizard;
		team2[i] = Blastoise;
	}

	// Initialize trainer teams
	// Allocation for GPU
	PokemonData* h_p1 = new PokemonData;
	PokemonData* h_p2 = new PokemonData;
	int* h_results = new int[MAX_POKEMON];
	int* turn_results = new int[MAX_POKEMON];

	// Initialization
	for (int i = 0; i < MAX_POKEMON; i++) {
		// Player 1 (Charmander)
		natureSet(i, Charizard);
		h_p1->nature[i] = Charizard.weirdo;
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
		h_p2->healthPoints[i] = Blastoise.healthPoints;
		h_p2->attack[i] = Blastoise.attack;
		h_p2->defense[i] = Blastoise.defense;
		h_p2->specialAttack[i] = Blastoise.specialAttack;
		h_p2->specialDefense[i] = Blastoise.specialDefense;
		h_p2->speed[i] = Blastoise.speed;
		h_p2->accuracy[i] = Blastoise.pokeacc;
		h_p2->evasion[i] = Blastoise.evasion;
		h_p2->type1[i] = Blastoise.type1;
		h_p2->type2[i] = Blastoise.type2;
		h_p2->nature[i] = Blastoise.weirdo;

		for (int j = 0; j < 4; j++) {
			h_p2->moves[i][j] = Blastoise.moves[j];
		}

		h_p2->statAtk[i][0] = Blastoise.statAtk[0];
		h_p2->statAtk[i][1] = Blastoise.statAtk[1];
		h_p2->statDef[i][0] = Blastoise.statDef[0];
		h_p2->statDef[i][1] = Blastoise.statDef[1];
		h_p2->statSpa[i][0] = Blastoise.statSpa[0];
		h_p2->statSpa[i][1] = Blastoise.statSpa[1];
		h_p2->statSpd[i][0] = Blastoise.statSpd[0];
		h_p2->statSpd[i][1] = Blastoise.statSpd[1];
		h_p2->statSpe[i][0] = Blastoise.statSpe[0];
		h_p2->statSpe[i][1] = Blastoise.statSpe[1];

		h_p2->statAcc[i][0] = Blastoise.statAcc[0];
		h_p2->statAcc[i][1] = Blastoise.statAcc[1];
		h_p2->statEva[i][0] = Blastoise.statEva[0];
		h_p2->statEva[i][1] = Blastoise.statEva[1];
	}

	// Run battles in series
	start = clock();
	for (int i = 0; i < ITERS; i++) {
		pokeBattleCPU(team1, team2, winner, MAX_POKEMON);
	}
	end = clock();
	tcpu = (float)(end - start) * 1000 / (float)CLOCKS_PER_SEC / ITERS;

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<> dist(0, 100);

	int seed = rng() % 10000;

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
	//std::cout.rdbuf(oldBuf);

	float speedup = tcpu / tgpu;
	// CPU results
	int cpuwin = 0;
	for (int i = 0; i < MAX_POKEMON; i++) {
		//printf("Winner CPU %s\n", winner[i].Pokename);
		if (std::strcmp(winner[i].Pokename, Blastoise.getPokeName()) == 0) {
			cpuwin++;
		}
	}
	
	int groupSize = MAX_POKEMON / 25;
	int group = 0;
	double natureWins[25];

	// GPU results
	int gpuwin = 0;
	for (int i = 0; i < MAX_POKEMON; i++) {
		if (h_results[i] == 1) {
			gpuwin++;
		}
		// After the wins for each nature have been counted, evaluate the win percentage
		if (i % groupSize == 0 && i!= 0) {
			natureWins[group] = (static_cast<double>(gpuwin) / groupSize) * 100;
			group++;
			gpuwin = 0;
		}
	}
	// Calculate the win percentage for the last group
	natureWins[group] = (static_cast<double>(gpuwin) / (groupSize)) * 100;

	//natureWins[group] = (static_cast<double>(gpuwin) / groupSize) * 100;

	//double win = (static_cast<double>(gpuwin) / MAX_POKEMON) * 100;

	group = 0;
	int avg = 0;
	int natureAvg[25];
	for (int i = 0; i < MAX_POKEMON; i++) {
		avg += turn_results[i];
		if (i % groupSize == 0 && i!= 0) {
			natureAvg[group] = avg / groupSize;
			group++;
			avg = 0;
		}
	}
	// avg /= MAX_POKEMON;
	// Calculate the avg turns for the last group
	natureAvg[group] = avg / groupSize;

	char natureNames[25][20] = {
		"Hardy", "Lonely", "Brave", "Adamant", "Naughty",
		"Bold", "Docile", "Relaxed", "Impish", "Lax",
		"Timid", "Hasty", "Serious", "Jolly", "Naive",
		"Modest", "Mild", "Quiet", "Bashful", "Rash",
		"Calm", "Gentle", "Sassy", "Careful", "Quirky"
	};

	printf("CPU time: %e\n", tcpu);
	printf("GPU time: %e\n", tgpu);
	printf("Speedup: %e\n", speedup);
	printf("Win Percentage of %s:\n", Charizard.getPokeName());
	for (int i = 0; i < 25; i++) {
		printf("\tNature %s: %f %%\n", natureNames[i], natureWins[i]);
		printf("\tAverage number of turns: %d\n\n", natureAvg[i]);
	}

	//printf("Average number of turns: %d", avg);

	/*outFile << "Pokemon Size: " << MAX_POKEMON << "\n";
	outFile << "CPU time: " << tcpu << " ms\n";
	outFile << "GPU time: " << tgpu << " ms\n";
	outFile << "Speedup: " << speedup << "\n";
	outFile << std::scientific;
	outFile << std::fixed << std::setprecision(2);
	outFile << "Win Percentage of" << " Charizard: " << win << " %\n " << std::endl;
	outFile.close();*/

	delete[] team1;
	delete[] team2;
	delete[] winner;
	delete[] h_p1;
	delete[] h_p2;
	delete[] h_results;
	delete[] turn_results;
	return 0;
} // main