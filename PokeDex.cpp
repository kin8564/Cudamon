#include <iostream>
#include <array>
#include <string>
#include <optional>
#include "Move.h"
#include "Types.h"
#include "Pokemon.h"
#include "AttackDex.h"

class PokeDex{
    public:
        std::array<Pokemon*, 151> pokedex;

        void populate(){
            static AttackDex attackDex;
            attackDex.populate();

            pokedex[0] = new Pokemon("Dummy", 200, 200, 200, 200, 200, 200, Types::DRAGON, Types::PSYCHIC);
            pokedex[1] = new Pokemon("Bulbasaur", 152, 111, 111, 128, 128, 106, Types::GRASS, Types::POISON);
            pokedex[1]->setMove1(attackDex.attackdex["Tackle"]);
            pokedex[1]->setMove2(attackDex.attackdex["Growl"]);
            pokedex[1]->setMove3(attackDex.attackdex["Vine Whip"]);
            pokedex[1]->setMove4(attackDex.attackdex["Growth"]);
            
            pokedex[4] = new Pokemon("Charmander", 146, 114, 104, 123, 112, 128, Types::FIRE, Types::NONE);
            pokedex[4]->setMove1(attackDex.attackdex["Scratch"]);
            pokedex[4]->setMove2(attackDex.attackdex["Growl"]);
            pokedex[4]->setMove3(attackDex.attackdex["Ember"]);
            pokedex[4]->setMove4(attackDex.attackdex["Smokescreen"]);

            pokedex[7] = new Pokemon("Squirtle", 151, 110, 128, 112, 127, 104, Types::WATER, Types::NONE);
            pokedex[7]->setMove1(attackDex.attackdex["Tackle"]);
            pokedex[7]->setMove2(attackDex.attackdex["Tail Whip"]);
            pokedex[7]->setMove3(attackDex.attackdex["Water Gun"]);
            pokedex[7]->setMove4(attackDex.attackdex["Withdraw"]);
        }

};