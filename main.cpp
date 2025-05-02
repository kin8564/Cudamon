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
const int numNatures = 25;

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
	////std::fstream outFile("E:/Cudamon/results.txt", std::ios::app);
	//if (!outFile) {
	//	std::cerr << "Error opening file for writing!" << std::endl;
	//	return 1;
	//}

	//outFile << std::scientific;

	clock_t start, end;
	float tcpu, tgpu;
	AttackDex attackdex;
	attackdex.populate();
	PokeDex pokedex;
	pokedex.dexpopulate();

	Pokemon Charmander = pokedex.pokedex[4];
	Pokemon Squirtle = pokedex.pokedex[7];
	Pokemon Venusaur = pokedex.pokedex[3];
	Pokemon Charizard = pokedex.pokedex[6];
	Pokemon Blastoise = pokedex.pokedex[9];
	Pokemon Pidgeot = pokedex.pokedex[18];
	Pokemon Raichu = pokedex.pokedex[26];
	Pokemon Alakazam = pokedex.pokedex[65];
	Pokemon Machamp = pokedex.pokedex[68];
	Pokemon Golem = pokedex.pokedex[76];
	Pokemon Gengar = pokedex.pokedex[94];
	Pokemon Pinsir = pokedex.pokedex[127];
	Pokemon Snorlax = pokedex.pokedex[143];
	Pokemon Dragonite = pokedex.pokedex[149];
	Pokemon Mewtwo = pokedex.pokedex[150];
	Pokemon Mew = pokedex.pokedex[151];

	// CPU
	Pokemon* team1 = new Pokemon[MAX_POKEMON];
	Pokemon* team2 = new Pokemon[MAX_POKEMON];
	Pokemon* winner = new Pokemon[MAX_POKEMON];

	for (int i = 0; i < MAX_POKEMON; i++) {
		team1[i] = Raichu;
		natureSet(i, team1[i]);
		team2[i] = Pinsir;
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
		natureSet(i, Raichu);
		h_p1->nature[i] = Raichu.weirdo;
		h_p1->healthPoints[i] = Raichu.healthPoints;
		h_p1->attack[i] = Raichu.attack;
		h_p1->defense[i] = Raichu.defense;
		h_p1->specialAttack[i] = Raichu.specialAttack;
		h_p1->specialDefense[i] = Raichu.specialDefense;
		h_p1->speed[i] = Raichu.speed;
		h_p1->accuracy[i] = Raichu.pokeacc;
		h_p1->evasion[i] = Raichu.evasion;
		h_p1->type1[i] = Raichu.type1;
		h_p1->type2[i] = Raichu.type2;

		for (int j = 0; j < 4; j++) {
			h_p1->moves[i][j] = Raichu.moves[j];
		}

		h_p1->statAtk[i][0] = Raichu.statAtk[0];
		h_p1->statAtk[i][1] = Raichu.statAtk[1];
		h_p1->statDef[i][0] = Raichu.statDef[0];
		h_p1->statDef[i][1] = Raichu.statDef[1];
		h_p1->statSpa[i][0] = Raichu.statSpa[0];
		h_p1->statSpa[i][1] = Raichu.statSpa[1];
		h_p1->statSpd[i][0] = Raichu.statSpd[0];
		h_p1->statSpd[i][1] = Raichu.statSpd[1];
		h_p1->statSpe[i][0] = Raichu.statSpe[0];
		h_p1->statSpe[i][1] = Raichu.statSpe[1];

		h_p1->statAcc[i][0] = Raichu.statAcc[0];
		h_p1->statAcc[i][1] = Raichu.statAcc[1];
		h_p1->statEva[i][0] = Raichu.statEva[0];
		h_p1->statEva[i][1] = Raichu.statEva[1];

		// Player 2 (Squirtle)
		h_p2->healthPoints[i] = Pinsir.healthPoints;
		h_p2->attack[i] = Pinsir.attack;
		h_p2->defense[i] = Pinsir.defense;
		h_p2->specialAttack[i] = Pinsir.specialAttack;
		h_p2->specialDefense[i] = Pinsir.specialDefense;
		h_p2->speed[i] = Pinsir.speed;
		h_p2->accuracy[i] = Pinsir.pokeacc;
		h_p2->evasion[i] = Pinsir.evasion;
		h_p2->type1[i] = Pinsir.type1;
		h_p2->type2[i] = Pinsir.type2;
		h_p2->nature[i] = Pinsir.weirdo;

		for (int j = 0; j < 4; j++) {
			h_p2->moves[i][j] = Pinsir.moves[j];
		}

		h_p2->statAtk[i][0] = Pinsir.statAtk[0];
		h_p2->statAtk[i][1] = Pinsir.statAtk[1];
		h_p2->statDef[i][0] = Pinsir.statDef[0];
		h_p2->statDef[i][1] = Pinsir.statDef[1];
		h_p2->statSpa[i][0] = Pinsir.statSpa[0];
		h_p2->statSpa[i][1] = Pinsir.statSpa[1];
		h_p2->statSpd[i][0] = Pinsir.statSpd[0];
		h_p2->statSpd[i][1] = Pinsir.statSpd[1];
		h_p2->statSpe[i][0] = Pinsir.statSpe[0];
		h_p2->statSpe[i][1] = Pinsir.statSpe[1];

		h_p2->statAcc[i][0] = Pinsir.statAcc[0];
		h_p2->statAcc[i][1] = Pinsir.statAcc[1];
		h_p2->statEva[i][0] = Pinsir.statEva[0];
		h_p2->statEva[i][1] = Pinsir.statEva[1];
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
	
	int groupSize = MAX_POKEMON / numNatures;
	int group = 0;
	double natureWins[numNatures];

	// GPU results
	int gpuwin = 0;
	for (int i = 0; i < MAX_POKEMON; i++) {
		if (h_results[i] == 1) {
			gpuwin++;
		}
		// After the wins for each nature have been counted, evaluate the win percentage
		if ((i + 1) % groupSize == 0) {
			natureWins[group] = (static_cast<double>(gpuwin) / groupSize) * 100;
			group++;
			gpuwin = 0;
		}
	}

	group = 0;
	int avg = 0;
	int natureAvg[numNatures];
	for (int i = 0; i < MAX_POKEMON; i++) {
		avg += turn_results[i];
		if ((i + 1) % groupSize == 0) {
			natureAvg[group] = avg / groupSize;
			group++;
			avg = 0;
		}
	}

	char natureNames[numNatures][20] = {
		"Hardy", "Lonely", "Brave", "Adamant", "Naughty",
		"Bold", "Docile", "Relaxed", "Impish", "Lax",
		"Timid", "Hasty", "Serious", "Jolly", "Naive",
		"Modest", "Mild", "Quiet", "Bashful", "Rash",
		"Calm", "Gentle", "Sassy", "Careful", "Quirky"
	};

	printf("%s vs %s\n\n", team1[0].getPokeName(), team2[0].getPokeName());
	printf("CPU time: %e\n", tcpu);
	printf("GPU time: %e\n", tgpu);
	printf("Speedup: %e\n", speedup);
	double highestWin = 0.0;
	char bestNature[20];
	for (int i = 0; i < numNatures; i++) {
		double currentWin = natureWins[i];
		if (currentWin > highestWin) {
			highestWin = currentWin;
			strncpy_s(bestNature, sizeof(natureNames[i]), natureNames[i], 20);
		}
	}
	printf("Best Nature: %s\n", bestNature);
	printf("Win Percentage of %s by Nature:\n", team1[0].getPokeName());
	for (int i = 0; i < numNatures; i++) {
		printf("\tNature %s: %f %%\n", natureNames[i], natureWins[i]);
		printf("\tAverage number of turns: %d\n\n", natureAvg[i]);
	}

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