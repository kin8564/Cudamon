#ifndef PokeDex_H
#define PokeDex_H

#include <iostream>
#include <array>
#include <string>
#include "Move.h"
#include "Types.h"
#include "Pokemon.h"

class PokeDex{
    public:
        std::array<Pokemon*, 151> pokedex;

        void populate();

};

#endif