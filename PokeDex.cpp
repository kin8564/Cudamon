#include <iostream>
#include <array>
#include <string>
#include "Move.h"
#include "Types.h"
#include "Pokemon.h"
#include "AttackDex.h"
#include "PokeDex.h"

Pokemon pokedex[151];

PokeDex::PokeDex(){}

void PokeDex::dexpopulate() {
    static AttackDex attackDex;
    attackDex.populate();

    pokedex[0] = Pokemon("Dummy", 200, 200, 200, 200, 200, 200, TYPES::DRAGON, TYPES::PSYCHIC);
    pokedex[1] = Pokemon("Bulbasaur", 152, 111, 111, 128, 128, 106, TYPES::GRASS, TYPES::POISON);
    pokedex[1].setMove1(attackDex.attackdex["Tackle"]);
    pokedex[1].setMove2(attackDex.attackdex["Growl"]);
    pokedex[1].setMove3(attackDex.attackdex["Vine Whip"]);
    pokedex[1].setMove4(attackDex.attackdex["Growth"]);
    
    pokedex[4] = Pokemon("Charmander", 146, 114, 104, 123, 112, 128, TYPES::FIRE, TYPES::NONE);
    pokedex[4].setMove1(attackDex.attackdex["Scratch"]);
    pokedex[4].setMove2(attackDex.attackdex["Growl"]);
    pokedex[4].setMove3(attackDex.attackdex["Ember"]);
    pokedex[4].setMove4(attackDex.attackdex["Smokescreen"]);

    pokedex[7] = Pokemon("Squirtle", 151, 110, 128, 112, 127, 104, TYPES::WATER, TYPES::NONE);
    pokedex[7].setMove1(attackDex.attackdex["Tackle"]);
    pokedex[7].setMove2(attackDex.attackdex["Tail Whip"]);
    pokedex[7].setMove3(attackDex.attackdex["Water Gun"]);
    pokedex[7].setMove4(attackDex.attackdex["Withdraw"]);
}