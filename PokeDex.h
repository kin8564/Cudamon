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
        PokeDex();
        Pokemon pokedex[151];

        void dexpopulate();

};

#endif