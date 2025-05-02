#include <iostream>
#include <unordered_map>
#include <string>
#include "Move.h"
#include "Types.h"
#include "AttackDex.h"

void AttackDex::populate() {
    int empty[8] = {0};  // default zeroed effect

	// Name, Type, Category, Power, Accuracy, PP, StatEffect
    attackdex["Tackle"] = Move("Tackle", TYPES::NORMAL, 1, 40, 100, 35, empty);
    attackdex["Scratch"] = Move("Scratch", TYPES::NORMAL, 1, 40, 100, 35, empty);

	// Self/Opponent, Attack, Defense, Special Attack, Special Defense, Speed, Accuracy, Evasion
    int growl[8] = {1, -1, 0, 0, 0, 0, 0, 0}; // lower target attack
    attackdex["Growl"] = Move("Growl", TYPES::NORMAL, 3, 0, 100, 40, growl);

    int tailWhip[8] = {1, 0, -1, 0, 0, 0, 0, 0}; // lower target defense
    attackdex["Tail Whip"] = Move("Tail Whip", TYPES::NORMAL, 3, 0, 100, 40, tailWhip);

    attackdex["Vine Whip"] = Move("Vine Whip", TYPES::GRASS, 1, 45, 100, 25, empty);
	attackdex["Razor Leaf"] = Move("Razor Leaf", TYPES::GRASS, 1, 55, 95, 25, empty);
	attackdex["Petal Blizzard"] = Move("Petal Blizzard", TYPES::GRASS, 1, 90, 100, 15, empty);
	attackdex["Seed Bomb"] = Move("Seed Bomb", TYPES::GRASS, 1, 80, 100, 15, empty);
	attackdex["Take Down"] = Move("Take Down", TYPES::NORMAL, 1, 90, 85, 20, empty);
	int sweetScent[8] = { 1, 0, 0, 0, 0, 0, 0, -1 }; // lower target evasion
	attackdex["Sweet Scent"] = Move("Sweet Scent", TYPES::NORMAL, 3, 0, 100, 20, sweetScent);
	attackdex["Energy Ball"] = Move("Energy Ball", TYPES::GRASS, 2, 90, 100, 10, empty);

    attackdex["Water Gun"] = Move("Water Gun", TYPES::WATER, 2, 40, 100, 25, empty);
    attackdex["Ember"] = Move("Ember", TYPES::FIRE, 2, 40, 100, 25, empty);

    int growth[8] = {0, 1, 0, 1, 0, 0, 0, 0}; // raise user atk and spa
    attackdex["Growth"] = Move("Growth", TYPES::NORMAL, 3, 0, 0, 20, growth);

    int withdraw[8] = {0, 0, 1, 0, 0, 0, 0, 0}; // raise user defense
    attackdex["Withdraw"] = Move("Withdraw", TYPES::WATER, 3, 0, 100, 40, withdraw);

    int smokescreen[8] = {1, 0, 0, 0, 0, 0, -1, 0}; // lower target accuracy
    attackdex["Smokescreen"] = Move("Smokescreen", TYPES::NORMAL, 3, 0, 100, 20, smokescreen);
	attackdex["Flamethrower"] = Move("Flamethrower", TYPES::FIRE, 2, 90, 100, 15, empty);
	attackdex["Air Slash"] = Move("Air Slash", TYPES::FLYING, 2, 75, 95, 15, empty);
	attackdex["Dragon Claw"] = Move("Dragon Claw", TYPES::DRAGON, 1, 80, 100, 15, empty);

	attackdex["Hydro Pump"] = Move("Hydro Pump", TYPES::WATER, 2, 110, 80, 5, empty);
	attackdex["Surf"] = Move("Surf", TYPES::WATER, 2, 90, 100, 15, empty);
	// Self/Opponent, Attack, Defense, Special Attack, Special Defense, Speed, Accuracy, Evasion
	int shellSmash[8] = {0, 1, -1, 1, -1, 1, 0, 0};
	attackdex["Shell Smash"] = Move("Shell Smash", TYPES::NORMAL, 3, 0, 100, 15, shellSmash);
	attackdex["Flash Cannon"] = Move("Flash Cannon", TYPES::STEEL, 2, 80, 100, 10, empty);

	attackdex["Hurricane"] = Move("Hurricane", TYPES::FLYING, 2, 110, 70, 10, empty);
	int featherDance[8] = {1, -2, 0, 0, 0, 0, 0, 0};
	attackdex["Feather Dance"] = Move("Feather Dance", TYPES::NORMAL, 3, 0, 100, 15, featherDance);

	attackdex["Earthquake"] = Move("Earthquake", TYPES::GROUND, 1, 100, 100, 10, empty);
	attackdex["Rock Slide"] = Move("Rock Slide", TYPES::ROCK, 1, 75, 90, 10, empty);
	attackdex["Stone Edge"] = Move("Stone Edge", TYPES::ROCK, 1, 100, 80, 5, empty);
	int rockPolish[8] = { 0, 0, 0, 0, 0, 2, 0, 0};
	attackdex["Rock Polish"] = Move("Rock Polish", TYPES::ROCK, 3, 0, 0, 20, rockPolish);

	attackdex["Psychic"] = Move("Psychic", TYPES::PSYCHIC, 2, 90, 100, 10, empty);
	int kinesis[8] = { 1, 0, 0, 0, 0, 0, -1, 0 };
	attackdex["Kinesis"] = Move("Kinesis", TYPES::NORMAL, 3, 0, 100, 15, kinesis);
	int calmMind[8] = { 0, 0, 0, 1, 1, 0, 0, 0 };
	attackdex["Calm Mind"] = Move("Kinesis", TYPES::NORMAL, 3, 0, 100, 20, calmMind);
	attackdex["Psycho Cut"] = Move("Psycho Cut", TYPES::PSYCHIC, 1, 70, 100, 20, empty);

	attackdex["Thunderbolt"] = Move("Thunderbolt", TYPES::ELECTRIC, 2, 90, 100, 15, empty);
	attackdex["Quick Attack"] = Move("Quick Attack", TYPES::NORMAL, 1, 40, 100, 30, empty);
	attackdex["Iron Tail"] = Move("Iron Tail", TYPES::STEEL, 1, 100, 75, 15, empty);
	int doubleTeam[8] = { 0, 0, 0, 0, 0, 0, 0, 1 };
	attackdex["Double Team"] = Move("Double Team", TYPES::NORMAL, 3, 0, 100, 15, doubleTeam);
	
	attackdex["Mega Punch"] = Move("Mega Punch", TYPES::NORMAL, 1, 80, 85, 20, empty);
	int bulkUp[8] = { 0, 1, 1, 0, 0, 0, 0, 0};
	attackdex["Bulk Up"] = Move("Bulk Up", TYPES::FIGHTING, 3, 0, 100, 20, bulkUp);
	attackdex["Cross Chop"] = Move("Cross Chop", TYPES::FIGHTING, 1, 100, 80, 5, empty);
	attackdex["Bullet Punch"] = Move("Bullet Punch", TYPES::STEEL, 1, 40, 100, 20, empty);

	attackdex["Shadow Ball"] = Move("Shadow Ball", TYPES::GHOST, 2, 80, 100, 15, empty);
	attackdex["Dark Pulse"] = Move("Dark Pulse", TYPES::DARK, 2, 80, 100, 15, empty);
	attackdex["Confusion"] = Move("Confusion", TYPES::PSYCHIC, 2, 50, 100, 25, empty);
	int scaryFace[8] = { 1, 0, 0, 0, 0, -2, 0, 0 };
	attackdex["Scary Face"] = Move("Scary Face", TYPES::NORMAL, 3, 0, 100, 10, scaryFace);

	attackdex["Body Slam"] = Move("Body Slam", TYPES::NORMAL, 1, 85, 100, 15, empty);
	//Amnesia
	int amnesia[8] = { 0, 0, 0, 0, 2, 0, 0, 0 };
	attackdex["Amnesia"] = Move("Amnesia", TYPES::PSYCHIC, 3, 0, 100, 20, amnesia);
	attackdex["Hammer Arm"] = Move("Hammer Arm", TYPES::FIGHTING, 1, 100, 90, 10, empty);
	attackdex["Giga Impact"] = Move("Giga Impact", TYPES::NORMAL, 2, 150, 90, 5, empty);
	
	attackdex["Hyper Beam"] = Move("Hyper Beam", TYPES::NORMAL, 2, 150, 90, 5, empty);
	//Dragon Dance
	int dragonDance[8] = { 0, 1, 0, 0, 0, 1, 0, 0 };
	attackdex["Dragon Dance"] = Move("Dragon Dance", TYPES::DRAGON, 3, 0, 0, 20, dragonDance);
	//Hurricane
	attackdex["Hurricane"] = Move("Hurricane", TYPES::FLYING, 2, 110, 70, 10, empty);
	//Dragon Rush
	attackdex["Dragon Rush"] = Move("Dragon Rush", TYPES::DRAGON, 1, 110, 75, 10, empty);
	
	//X-Scissor
	attackdex["X-Scissor"] = Move("X-Scissor", TYPES::BUG, 1, 80, 100, 15, empty);
	//Strength
	attackdex["Strength"] = Move("Stength", TYPES::NORMAL, 1, 80, 100, 15, empty);
	//Swords Dance
	int swordsDance[8] = { 0, 2, 0, 0, 0, 0, 0, 0 };
	attackdex["Swords Dance"] = Move("Swords Dance", TYPES::NORMAL, 3, 0, 100, 20, swordsDance);
	//Harden
	int harden[8] = { 0, 0, 1, 0, 0, 0, 0, 0 };
	attackdex["Harden"] = Move("Harden", TYPES::NORMAL, 3, 0, 100, 30, harden);

	//Psystrike
	attackdex["Psystrike"] = Move("Psystrike", TYPES::PSYCHIC, 2, 100, 100, 10, empty);
	//Aura Sphere
	attackdex["Aura Sphere"] = Move("Aura Sphere", TYPES::FIGHTING, 2, 80, 100, 20, empty);
	//Ancient Power
	attackdex["Ancient Power"] = Move("Ancient Power", TYPES::ROCK, 2, 60, 100, 5, empty);

	//Nasty Plot
	int nastyPlot[8] = { 0, 0, 0, 2, 0, 0, 0, 0 };
	attackdex["Nasty Plot"] = Move("Nasty Plot", TYPES::DARK, 3, 0, 0, 20, nastyPlot);
	//Pound
	attackdex["Pound"] = Move("Pound", TYPES::NORMAL, 1, 40, 100, 35, empty);
}
