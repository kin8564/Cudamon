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

	// Buff/debuff, Attack, Defense, Special Attack, Special Defense, Speed, Accuracy, Evasion
    int growlEffect[8] = {1, -1, 0, 0, 0, 0, 0, 0}; // lower target attack
    attackdex["Growl"] = Move("Growl", TYPES::NORMAL, 3, 0, 100, 40, growlEffect);

    int tailWhipEffect[8] = {1, 0, -1, 0, 0, 0, 0, 0}; // lower target defense
    attackdex["Tail Whip"] = Move("Tail Whip", TYPES::NORMAL, 3, 0, 100, 40, tailWhipEffect);

    attackdex["Vine Whip"] = Move("Vine Whip", TYPES::GRASS, 1, 45, 100, 25, empty);
	attackdex["Razor Leaf"] = Move("Razor Leaf", TYPES::GRASS, 1, 55, 95, 25, empty);
	attackdex["Petal Blizzard"] = Move("Petal Blizzard", TYPES::GRASS, 1, 90, 100, 15, empty);
	attackdex["Seed Bomb"] = Move("Seed Bomb", TYPES::GRASS, 1, 80, 100, 15, empty);
	attackdex["Take Down"] = Move("Take Down", TYPES::NORMAL, 1, 90, 85, 20, empty);
	int sweetScentEffect[8] = { 1, 0, 0, 0, 0, 0, 0, -1 }; // lower target evasion
	attackdex["Sweet Scent"] = Move("Sweet Scent", TYPES::NORMAL, 3, 0, 100, 20, sweetScentEffect);
	attackdex["Energy Ball"] = Move("Energy Ball", TYPES::GRASS, 2, 90, 100, 10, empty);

    attackdex["Water Gun"] = Move("Water Gun", TYPES::WATER, 2, 40, 100, 25, empty);
    attackdex["Ember"] = Move("Ember", TYPES::FIRE, 2, 40, 100, 25, empty);

    int growthEffect[8] = {0, 1, 0, 1, 0, 0, 0, 0}; // raise user atk and spa
    attackdex["Growth"] = Move("Growth", TYPES::NORMAL, 3, 0, 0, 20, growthEffect);

    int withdrawEffect[8] = {0, 0, 1, 0, 0, 0, 0, 0}; // raise user defense
    attackdex["Withdraw"] = Move("Withdraw", TYPES::WATER, 3, 0, 0, 40, withdrawEffect);

    int smokescreenEffect[8] = {1, 0, 0, 0, 0, 0, -1, 0}; // lower target accuracy
    attackdex["Smokescreen"] = Move("Smokescreen", TYPES::NORMAL, 3, 0, 100, 20, smokescreenEffect);
	attackdex["Flamethrower"] = Move("Flamethrower", TYPES::FIRE, 2, 90, 100, 15, empty);
	attackdex["Air Slash"] = Move("Air Slash", TYPES::FLYING, 2, 75, 95, 15, empty);
	attackdex["Dragon Claw"] = Move("Dragon Claw", TYPES::DRAGON, 1, 80, 100, 15, empty);

	attackdex["Hydro Pump"] = Move("Hydro Pump", TYPES::WATER, 2, 110, 80, 5, empty);
	attackdex["Surf"] = Move("Surf", TYPES::WATER, 2, 90, 100, 15, empty);
	attackdex["Earthquake"] = Move("Earthquake", TYPES::GROUND, 1, 100, 100, 10, empty);
	attackdex["Rock Slide"] = Move("Rock Slide", TYPES::ROCK, 1, 75, 90, 10, empty);
	attackdex["Psychic"] = Move("Psychic", TYPES::PSYCHIC, 2, 90, 100, 10, empty);
	attackdex["Thunderbolt"] = Move("Thunderbolt", TYPES::ELECTRIC, 2, 90, 100, 15, empty);
	attackdex["Hyper Beam"] = Move("Hyper Beam", TYPES::NORMAL, 1, 150, 90, 5, empty);
	attackdex["Seismic Toss"] = Move("Seismic Toss", TYPES::NORMAL, 1, 0, 100, 20, empty);
	attackdex["Confusion"] = Move("Confusion", TYPES::PSYCHIC, 2, 50, 100, 25, empty);
	attackdex["Double-Edge"] = Move("Double-Edge", TYPES::NORMAL, 1, 100, 100, 15, empty);
	attackdex["Body Slam"] = Move("Body Slam", TYPES::NORMAL, 1, 85, 100, 15, empty);
	attackdex["Mega Punch"] = Move("Mega Punch", TYPES::NORMAL, 1, 80, 85, 20, empty);
	attackdex["Mega Kick"] = Move("Mega Kick", TYPES::NORMAL, 1, 120, 75, 5, empty);

}
