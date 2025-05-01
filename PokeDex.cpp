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


    pokedex[4] = Pokemon("Charmander", 146, 114, 104, 123, 112, 128, TYPES::FIRE, TYPES::NONE);
    pokedex[4].setMove1(attackDex.attackdex["Scratch"]);
    pokedex[4].setMove2(attackDex.attackdex["Growl"]);
    pokedex[4].setMove3(attackDex.attackdex["Ember"]);
    pokedex[4].setMove4(attackDex.attackdex["Smokescreen"]);

	pokedex[6] = Pokemon("Charizard", 185, 149, 143, 177, 150, 167, TYPES::FIRE, TYPES::FLYING);

    pokedex[7] = Pokemon("Squirtle", 151, 110, 128, 112, 127, 104, TYPES::WATER, TYPES::NONE);
    pokedex[7].setMove1(attackDex.attackdex["Tackle"]);
    pokedex[7].setMove2(attackDex.attackdex["Tail Whip"]);
    pokedex[7].setMove3(attackDex.attackdex["Water Gun"]);
    pokedex[7].setMove4(attackDex.attackdex["Withdraw"]);
    
	pokedex[9] = Pokemon("Blastoise", 186, 148, 167, 150, 172, 143, TYPES::WATER, TYPES::NONE);

    //Pigeot
	pokedex[18] = Pokemon("Pidgeot", 190, 145, 139, 134, 134, 157, TYPES::NORMAL, TYPES::FLYING);

    //Raichu
	pokedex[26] = Pokemon("Raichu", 167, 156, 117, 156, 145, 167, TYPES::ELECTRIC, TYPES::NONE);
    
    //Alakazam - Check
	pokedex[65] = Pokemon("Alakazam", 167, 145, 128, 200, 128, 156, TYPES::PSYCHIC, TYPES::NONE);

    //Machamp
	pokedex[68] = Pokemon("Machamp", 197, 200, 145, 128, 150, 117, TYPES::FIGHTING, TYPES::NONE);

    //Golem
	pokedex[76] = Pokemon("Golem", 187, 178, 200, 117, 128, 106, TYPES::ROCK, TYPES::GROUND);

    //Gengar
	pokedex[94] = Pokemon("Gengar", 167, 128, 123, 200, 139, 178, TYPES::GHOST, TYPES::POISON);

    //Kangaskhan
	pokedex[115] = Pokemon("Kangaskhan", 212, 161, 145, 101, 145, 156, TYPES::NORMAL, TYPES::NONE);

    //Pinsir
	pokedex[127] = Pokemon("Pinsir", 172, 194, 167, 117, 134, 150, TYPES::BUG, TYPES::NONE);

    //Snorlax
	pokedex[143] = Pokemon("Snorlax", 267, 178, 128, 128, 178, 90, TYPES::NORMAL, TYPES::NONE);

    //Opponent
	pokedex[149] = Pokemon("Dragonite", 198, 204, 161, 167, 167, 145, TYPES::DRAGON, TYPES::FLYING);

    //Opponent
	pokedex[150] = Pokemon("Mewtwo", 213, 178, 156, 226, 156, 200, TYPES::PSYCHIC, TYPES::NONE);

	pokedex[151] = Pokemon("Mew", 207, 167, 167, 167, 167, 167, TYPES::PSYCHIC, TYPES::NONE);
	
}