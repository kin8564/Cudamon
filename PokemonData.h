#pragma once

#include "Move.h"
#include "Types.h"
#include "Move.h"
#include "Natures.h"

#define MAX_POKEMON 1000
#define MAX_MOVES 4

struct PokemonData {
    int healthPoints[MAX_POKEMON];
    int attack[MAX_POKEMON];
    int defense[MAX_POKEMON];
    int specialAttack[MAX_POKEMON];
    int specialDefense[MAX_POKEMON];
    int speed[MAX_POKEMON];
    int accuracy[MAX_POKEMON];
    int evasion[MAX_POKEMON];

    TYPES type1[MAX_POKEMON];
    TYPES type2[MAX_POKEMON];

    NATURES nature[MAX_POKEMON];

    Move moves[MAX_POKEMON][MAX_MOVES];
};

