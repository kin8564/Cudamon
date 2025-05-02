#include <iostream>
#include <array>
#include <string>
#include "Move.h"
#include "Types.h"
#include "Pokemon.h"
#include "AttackDex.h"
#include "PokeDex.h"

PokeDex::PokeDex(){}

void PokeDex::dexpopulate() {
    static AttackDex attackDex;
    attackDex.populate();
    //Multiple instances of the same pokemon, change moveset bwetween them
    pokedex[0] = Pokemon("missingno", 200, 200, 200, 200, 200, 200, TYPES::DRAGON, TYPES::PSYCHIC);

    pokedex[1] = Pokemon("Bulbasaur", 152, 111, 111, 128, 128, 106, TYPES::GRASS, TYPES::POISON);
    pokedex[1].setMove1(attackDex.attackdex["Tackle"]);
    pokedex[1].setMove2(attackDex.attackdex["Growl"]);
    pokedex[1].setMove3(attackDex.attackdex["Vine Whip"]);
    pokedex[1].setMove4(attackDex.attackdex["Growth"]);
    
	pokedex[3] = Pokemon("Venusaur", 187, 147, 148, 167, 167, 145, TYPES::GRASS, TYPES::POISON);
    pokedex[3].setMove1(attackDex.attackdex["Petal Blizzard"]);
    pokedex[3].setMove2(attackDex.attackdex["Take Down"]);
    pokedex[3].setMove3(attackDex.attackdex["Energy Ball"]);
    pokedex[3].setMove4(attackDex.attackdex["Sweet Scent"]);

    pokedex[4] = Pokemon("Charmander", 146, 114, 104, 123, 112, 128, TYPES::FIRE, TYPES::NONE);
    pokedex[4].setMove1(attackDex.attackdex["Scratch"]);
    pokedex[4].setMove2(attackDex.attackdex["Growl"]);
    pokedex[4].setMove3(attackDex.attackdex["Ember"]);
    pokedex[4].setMove4(attackDex.attackdex["Smokescreen"]);

	pokedex[6] = Pokemon("Charizard", 185, 149, 143, 177, 150, 167, TYPES::FIRE, TYPES::FLYING);
    pokedex[6].setMove1(attackDex.attackdex["Flamethrower"]);
    pokedex[6].setMove2(attackDex.attackdex["Air Slash"]);
    pokedex[6].setMove3(attackDex.attackdex["Dragon Claw"]);
    pokedex[6].setMove4(attackDex.attackdex["Smokescreen"]);

    pokedex[7] = Pokemon("Squirtle", 151, 110, 128, 112, 127, 104, TYPES::WATER, TYPES::NONE);
    pokedex[7].setMove1(attackDex.attackdex["Tackle"]);
    pokedex[7].setMove2(attackDex.attackdex["Tail Whip"]);
    pokedex[7].setMove3(attackDex.attackdex["Water Gun"]);
    pokedex[7].setMove4(attackDex.attackdex["Withdraw"]);
    
	pokedex[9] = Pokemon("Blastoise", 186, 148, 167, 150, 172, 143, TYPES::WATER, TYPES::NONE);
    pokedex[9].setMove1(attackDex.attackdex["Surf"]);
    pokedex[9].setMove2(attackDex.attackdex["Flash Cannon"]);
    pokedex[9].setMove3(attackDex.attackdex["Withdraw"]);
    pokedex[9].setMove4(attackDex.attackdex["Shell Smash"]);

    //Pigeot
	pokedex[18] = Pokemon("Pidgeot", 190, 145, 139, 134, 134, 157, TYPES::NORMAL, TYPES::FLYING);
    pokedex[18].setMove1(attackDex.attackdex["Air Slash"]);
    pokedex[18].setMove2(attackDex.attackdex["Take Down"]);
    pokedex[18].setMove3(attackDex.attackdex["Hurricane"]);
    pokedex[18].setMove4(attackDex.attackdex["Feather Dance"]);

    //Raichu
	pokedex[26] = Pokemon("Raichu", 167, 156, 117, 156, 145, 167, TYPES::ELECTRIC, TYPES::NONE);
    pokedex[26].setMove1(attackDex.attackdex["Thunderbolt"]);
    pokedex[26].setMove2(attackDex.attackdex["Quick Attack"]);
    pokedex[26].setMove3(attackDex.attackdex["Iron Tail"]);
    pokedex[26].setMove4(attackDex.attackdex["Double Team"]);
    
    //Alakazam
	pokedex[65] = Pokemon("Alakazam", 162, 112, 106, 205, 150, 189, TYPES::PSYCHIC, TYPES::NONE);
    pokedex[65].setMove1(attackDex.attackdex["Psychic"]);
    pokedex[65].setMove2(attackDex.attackdex["Psycho Cut"]);
    pokedex[65].setMove3(attackDex.attackdex["Calm Mind"]);
    pokedex[65].setMove4(attackDex.attackdex["Kinesis"]);

    //Machamp
	pokedex[68] = Pokemon("Machamp", 197, 200, 145, 128, 150, 117, TYPES::FIGHTING, TYPES::NONE);
    pokedex[68].setMove1(attackDex.attackdex["Cross Chop"]);
    pokedex[68].setMove2(attackDex.attackdex["Mega Punch"]);
    pokedex[68].setMove3(attackDex.attackdex["Bullet Punch"]);
    pokedex[68].setMove4(attackDex.attackdex["Bulk Up"]);

    //Golem
	pokedex[76] = Pokemon("Golem", 187, 178, 200, 117, 128, 106, TYPES::ROCK, TYPES::GROUND);
    pokedex[76].setMove1(attackDex.attackdex["Earthquake"]);
    pokedex[76].setMove2(attackDex.attackdex["Rock Slide"]);
    pokedex[76].setMove3(attackDex.attackdex["Stone Edge"]);
    pokedex[76].setMove4(attackDex.attackdex["Rock Polish"]);

    //Gengar
	pokedex[94] = Pokemon("Gengar", 167, 128, 123, 200, 139, 178, TYPES::GHOST, TYPES::POISON);
    pokedex[94].setMove1(attackDex.attackdex["Shadow Ball"]);
    pokedex[94].setMove2(attackDex.attackdex["Dark Pulse"]);
    pokedex[94].setMove3(attackDex.attackdex["Confusion"]);
    pokedex[94].setMove4(attackDex.attackdex["Scary Face"]);

    //Pinsir
	pokedex[127] = Pokemon("Pinsir", 172, 194, 167, 117, 134, 150, TYPES::BUG, TYPES::NONE);
    pokedex[127].setMove1(attackDex.attackdex["X-Scissor"]);
    pokedex[127].setMove2(attackDex.attackdex["Strength"]);
    pokedex[127].setMove3(attackDex.attackdex["Swords Dance"]);
    pokedex[127].setMove4(attackDex.attackdex["Harden"]);

    //Snorlax
	pokedex[143] = Pokemon("Snorlax", 267, 178, 128, 128, 178, 90, TYPES::NORMAL, TYPES::NONE);
    pokedex[18].setMove1(attackDex.attackdex["Body Slam"]);
    pokedex[18].setMove2(attackDex.attackdex["Giga Impact"]);
    pokedex[18].setMove3(attackDex.attackdex["Hammer Arm"]);
    pokedex[18].setMove4(attackDex.attackdex["Amnesia"]);

    //Dragonite
	pokedex[149] = Pokemon("Dragonite", 198, 204, 161, 167, 167, 145, TYPES::DRAGON, TYPES::FLYING);
    pokedex[149].setMove1(attackDex.attackdex["Hyper Beam"]);
    pokedex[149].setMove2(attackDex.attackdex["Dragon Rush"]);
    pokedex[149].setMove3(attackDex.attackdex["Hurricane"]);
    pokedex[149].setMove4(attackDex.attackdex["Dragon Dance"]);

    //Mewtwo
	pokedex[150] = Pokemon("Mewtwo", 213, 178, 156, 226, 156, 200, TYPES::PSYCHIC, TYPES::NONE);
    pokedex[150].setMove1(attackDex.attackdex["Psystrike"]);
    pokedex[150].setMove2(attackDex.attackdex["Aura Sphere"]);
    pokedex[150].setMove3(attackDex.attackdex["Ancient Power"]);
    pokedex[150].setMove4(attackDex.attackdex["Amnesia"]);

    //Mew
	pokedex[151] = Pokemon("Mew", 207, 167, 167, 167, 167, 167, TYPES::PSYCHIC, TYPES::NONE);
    pokedex[151].setMove1(attackDex.attackdex["Psychic"]);
    pokedex[151].setMove2(attackDex.attackdex["Mega Punch"]);
    pokedex[151].setMove3(attackDex.attackdex["Pound"]);
    pokedex[151].setMove4(attackDex.attackdex["Nasty Plot"]);

    
	
}