#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <curand_kernel.h>

#ifndef COMMON_H
#include "common.h"
#endif

#include <iostream>
#include <unordered_map>
#include <random>
#include <string>
#include "Move.h"
#include "TYPES.h"
#include "Pokemon.h"
#include "PokeDex.h"
#include "AttackDex.h"
#include "Slowdown.h"
#include <vector>
#include <random>
#include <thread>
#include <chrono>

const int TILE_SIZE = 16;
__device__ static int dmgArray[4];

__constant__ double statMultiplierGPU[13] = {
    2.0 / 8.0, 2.0 / 7.0, 2.0 / 6.0,
    2.0 / 5.0, 2.0 / 4.0, 2.0 / 3.0,
    2.0 / 2.0,
    3.0 / 2.0, 4.0 / 2.0, 5.0 / 2.0,
    6.0 / 2.0, 7.0 / 2.0, 8.0 / 2.0
};

__constant__ double accMultiplierGPU[13] = {
    3.0 / 9.0, 3.0 / 8.0, 3.0 / 7.0,
    3.0 / 6.0, 3.0 / 5.0, 3.0 / 4.0,
    3.0 / 3.0,
    4.0 / 3.0, 5.0 / 3.0, 6.0 / 3.0,
    7.0 / 3.0, 8.0 / 3.0, 9.0 / 3.0
};

__constant__ double evaMultiplierGPU[13] = {
    9.0 / 3.0, 8.0 / 3.0, 7.0 / 3.0,
    6.0 / 3.0, 5.0 / 3.0, 4.0 / 3.0,
    3.0 / 3.0,
    3.0 / 4.0, 3.0 / 5.0, 3.0 / 6.0,
    3.0 / 7.0, 3.0 / 8.0, 3.0 / 9.0
};


// GPU-compatible random number generation
__device__ int getRandom(int seed, int offset) {
    curandState state;
    curand_init(seed, offset, 0, &state);
    return curand(&state) % 100;
}

__device__ static void typeMultiplierGPU(int damage, Move move, Pokemon defender) {
    double modifier = 1.0;
    int superEff = 0;
    int notVeryEff = 0;
    int noEff = 0;

    // Check if defender has a primary type
    if (defender.type1 != TYPES::NONE) { // Adjust as necessary to handle null or no type
        switch (defender.type1) {
        case TYPES::BUG:
            // Weak: Flying, Rock, Fire
            if (move.type == TYPES::FLYING || move.type == TYPES::ROCK || move.type == TYPES::FIRE) {
                modifier *= 2;
            }
            // Resist: Fighting, Ground, Grass
            if (move.type == TYPES::FIGHTING || move.type == TYPES::GROUND || move.type == TYPES::GRASS) {
                modifier /= 2;
            }
            break;

        case TYPES::DARK:
            //Weak: BUG, FAIRY, FIGHTING
            if (move.type == TYPES::BUG || move.type == TYPES::FAIRY || move.type == TYPES::FIGHTING) {
                modifier = modifier * 2;
            }
            //Resist: GHOST, DARK
            if (move.type == TYPES::GHOST || move.type == TYPES::DARK) {
                modifier = modifier / 2;
            }
            //Immune: Psychic
            if (move.type == TYPES::PSYCHIC) {
                modifier = 0;
            }
            break;

        case TYPES::DRAGON:
            //Weak: DRAGON, FAIRY, ICE
            if (move.type == TYPES::DRAGON || move.type == TYPES::FAIRY || move.type == TYPES::ICE) {
                modifier = modifier * 2;
            }
            //Resist: FIRE, WATER, GRASS, ELECTRIC
            if (move.type == TYPES::FIRE || move.type == TYPES::WATER || move.type == TYPES::GRASS || move.type == TYPES::ELECTRIC) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::ELECTRIC:
            //Weak: GROUND
            if (move.type == TYPES::GROUND) {
                modifier = modifier * 2;
            }
            //Resist: FLYING, ELECTRIC, STEEL
            if (move.type == TYPES::FLYING || move.type == TYPES::ELECTRIC || move.type == TYPES::STEEL) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FAIRY:
            //Weak: POISON, STEEL
            if (move.type == TYPES::POISON || move.type == TYPES::STEEL) {
                modifier = modifier * 2;
            }
            //Resist: BUG, DARK, FIGHTING
            if (move.type == TYPES::BUG || move.type == TYPES::DARK || move.type == TYPES::FIGHTING) {
                modifier = modifier / 2;
            }
            //Immune: DRAGON
            if (move.type == TYPES::DRAGON) {
                modifier = 0;
            }
            break;

        case TYPES::FIGHTING:
            //Weak: FAIRY, FLYING, PSYCHIC
            if (move.type == TYPES::FAIRY || move.type == TYPES::FLYING || move.type == TYPES::PSYCHIC) {
                modifier = modifier * 2;
            }
            //Resist: BUG, DARK, ROCK
            if (move.type == TYPES::BUG || move.type == TYPES::DARK || move.type == TYPES::ROCK) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FIRE:
            //Weak: Ground, Rock, Water
            if (move.type == TYPES::GROUND || move.type == TYPES::ROCK || move.type == TYPES::WATER) {
                modifier = modifier * 2;
            }
            //Resist: Bug, Steel, Fire, Grass, Ice, Fairy
            if (move.type == TYPES::BUG || move.type == TYPES::STEEL || move.type == TYPES::FIRE ||
                move.type == TYPES::GRASS || move.type == TYPES::ICE || move.type == TYPES::FAIRY) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FLYING:
            //Weak: Electric, Ice, Rock
            if (move.type == TYPES::ELECTRIC || move.type == TYPES::ICE || move.type == TYPES::ROCK) {
                modifier = modifier * 2;
            }
            //Resist: Bug, Fighting, Grass
            if (move.type == TYPES::BUG || move.type == TYPES::FIGHTING || move.type == TYPES::GRASS) {
                modifier = modifier / 2;
            }
            //Immune: Ground
            if (move.type == TYPES::GROUND) {
                modifier = 0;
            }
            break;

        case TYPES::GHOST:
            //Weak: DARK, GHOST
            if (move.type == TYPES::DARK || move.type == TYPES::GHOST) {
                modifier = modifier * 2;
            }
            //Resist: BUG, POISON
            if (move.type == TYPES::BUG || move.type == TYPES::POISON) {
                modifier = modifier / 2;
            }
            //Immune: NORMAL, FIGHTING
            if (move.type == TYPES::NORMAL || move.type == TYPES::FIGHTING) {
                modifier = 0;
            }
            break;

        case TYPES::GRASS:
            //Weak: Bug, Fire, Flying, Ice, Poison
            if (move.type == TYPES::BUG || move.type == TYPES::FIRE || move.type == TYPES::FLYING ||
                move.type == TYPES::ICE || move.type == TYPES::POISON) {
                modifier = modifier * 2;
            }
            //Resist: Electric, Grass, Ground, Water
            if (move.type == TYPES::ELECTRIC || move.type == TYPES::GRASS || move.type == TYPES::GROUND || move.type == TYPES::WATER) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::GROUND:
            //Weak: WATER, GRASS, ICE
            if (move.type == TYPES::WATER || move.type == TYPES::GRASS || move.type == TYPES::ICE) {
                modifier = modifier * 2;
            }
            //Resist: POISON, ROCK
            if (move.type == TYPES::POISON || move.type == TYPES::ROCK) {
                modifier = modifier / 2;
            }
            //Immune: ELECTRIC
            if (move.type == TYPES::ELECTRIC) {
                modifier = 0;
            }
            break;

        case TYPES::ICE:
            //Weak: FIGHTING, ROCK, STEEL, FIRE
            if (move.type == TYPES::FIGHTING || move.type == TYPES::ROCK || move.type == TYPES::STEEL || move.type == TYPES::FIRE) {
                modifier = modifier * 2;
            }
            //Resist: ICE
            if (move.type == TYPES::ICE) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::NORMAL:
            //Weak: FIGHTING
            if (move.type == TYPES::FIGHTING) {
                modifier = modifier * 2;
            }
            //Immune: GHOST
            if (move.type == TYPES::GHOST) {
                modifier = 0;
            }
            break;

        case TYPES::POISON:
            //Weak: Ground, Psychic
            if (move.type == TYPES::GROUND || move.type == TYPES::PSYCHIC) {
                modifier = modifier * 2;
            }
            //Resist: Fighting, Poison, Bug, Grass, FAIRY
            if (move.type == TYPES::FIGHTING || move.type == TYPES::POISON || move.type == TYPES::BUG ||
                move.type == TYPES::GRASS || move.type == TYPES::FAIRY) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::PSYCHIC:
            //Weak: BUG, DARK, GHOST
            if (move.type == TYPES::FIGHTING || move.type == TYPES::POISON || move.type == TYPES::BUG) {
                modifier = modifier * 2;
            }
            //Resist: FIGHTING, PSYCHIC
            if (move.type == TYPES::FIGHTING || move.type == TYPES::PSYCHIC) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::ROCK:
            //Weak: FIGHTING, GROUND, STEEL, WATER, GRASS
            if (move.type == TYPES::FIGHTING || move.type == TYPES::GROUND || move.type == TYPES::STEEL ||
                move.type == TYPES::GRASS || move.type == TYPES::GRASS) {
                modifier = modifier * 2;
            }
            //Resist: NORMAL, FLYING, POISON, FIRE
            if (move.type == TYPES::NORMAL || move.type == TYPES::FLYING || move.type == TYPES::POISON || move.type == TYPES::FIRE) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::STEEL:
            //Weak: GROUND, FIGHTING, FIRE
            if (move.type == TYPES::GROUND || move.type == TYPES::FIGHTING || move.type == TYPES::FIRE) {
                modifier = modifier * 2;
            }
            //Resist: NORMAL, FLYING, ROCK, BUG, STEEL, GRASS, PSYCHIC, ICE, FAIRY, DRAGON
            if (move.type == TYPES::NORMAL || move.type == TYPES::FLYING || move.type == TYPES::ROCK || move.type == TYPES::BUG ||
                move.type == TYPES::STEEL || move.type == TYPES::GRASS || move.type == TYPES::PSYCHIC || move.type == TYPES::ICE ||
                move.type == TYPES::FAIRY || move.type == TYPES::DRAGON) {
                modifier = modifier / 2;
            }
            //Immune: POISON
            if (move.type == TYPES::POISON) {
                modifier = 0;
            }
            break;

        case TYPES::WATER:
            //Weak: Electric, Grass
            if (move.type == TYPES::ELECTRIC || move.type == TYPES::GRASS) {
                modifier = modifier * 2;
            }
            //Resist: Steel, Fire, Water, Ice
            if (move.type == TYPES::STEEL || move.type == TYPES::FIRE || move.type == TYPES::WATER || move.type == TYPES::ICE) {
                modifier = modifier / 2;
            }
            break;
        }
    }

    if (defender.type2 != TYPES::NONE) { // Adjust as necessary to handle null or no type
        switch (defender.type2) {
        case TYPES::BUG:
            // Weak: Flying, Rock, Fire
            if (move.type == TYPES::FLYING || move.type == TYPES::ROCK || move.type == TYPES::FIRE) {
                modifier *= 2;
            }
            // Resist: Fighting, Ground, Grass
            if (move.type == TYPES::FIGHTING || move.type == TYPES::GROUND || move.type == TYPES::GRASS) {
                modifier /= 2;
            }
            break;

        case TYPES::DARK:
            //Weak: BUG, FAIRY, FIGHTING
            if (move.type == TYPES::BUG || move.type == TYPES::FAIRY || move.type == TYPES::FIGHTING) {
                modifier = modifier * 2;
            }
            //Resist: GHOST, DARK
            if (move.type == TYPES::GHOST || move.type == TYPES::DARK) {
                modifier = modifier / 2;
            }
            //Immune: Psychic
            if (move.type == TYPES::PSYCHIC) {
                modifier = 0;
            }
            break;

        case TYPES::DRAGON:
            //Weak: DRAGON, FAIRY, ICE
            if (move.type == TYPES::DRAGON || move.type == TYPES::FAIRY || move.type == TYPES::ICE) {
                modifier = modifier * 2;
            }
            //Resist: FIRE, WATER, GRASS, ELECTRIC
            if (move.type == TYPES::FIRE || move.type == TYPES::WATER || move.type == TYPES::GRASS || move.type == TYPES::ELECTRIC) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::ELECTRIC:
            //Weak: GROUND
            if (move.type == TYPES::GROUND) {
                modifier = modifier * 2;
            }
            //Resist: FLYING, ELECTRIC, STEEL
            if (move.type == TYPES::FLYING || move.type == TYPES::ELECTRIC || move.type == TYPES::STEEL) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FAIRY:
            //Weak: POISON, STEEL
            if (move.type == TYPES::POISON || move.type == TYPES::STEEL) {
                modifier = modifier * 2;
            }
            //Resist: BUG, DARK, FIGHTING
            if (move.type == TYPES::BUG || move.type == TYPES::DARK || move.type == TYPES::FIGHTING) {
                modifier = modifier / 2;
            }
            //Immune: DRAGON
            if (move.type == TYPES::DRAGON) {
                modifier = 0;
            }
            break;

        case TYPES::FIGHTING:
            //Weak: FAIRY, FLYING, PSYCHIC
            if (move.type == TYPES::FAIRY || move.type == TYPES::FLYING || move.type == TYPES::PSYCHIC) {
                modifier = modifier * 2;
            }
            //Resist: BUG, DARK, ROCK
            if (move.type == TYPES::BUG || move.type == TYPES::DARK || move.type == TYPES::ROCK) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FIRE:
            //Weak: Ground, Rock, Water
            if (move.type == TYPES::GROUND || move.type == TYPES::ROCK || move.type == TYPES::WATER) {
                modifier = modifier * 2;
            }
            //Resist: Bug, Steel, Fire, Grass, Ice, Fairy
            if (move.type == TYPES::BUG || move.type == TYPES::STEEL || move.type == TYPES::FIRE ||
                move.type == TYPES::GRASS || move.type == TYPES::ICE || move.type == TYPES::FAIRY) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FLYING:
            //Weak: Electric, Ice, Rock
            if (move.type == TYPES::ELECTRIC || move.type == TYPES::ICE || move.type == TYPES::ROCK) {
                modifier = modifier * 2;
            }
            //Resist: Bug, Fighting, Grass
            if (move.type == TYPES::BUG || move.type == TYPES::FIGHTING || move.type == TYPES::GRASS) {
                modifier = modifier / 2;
            }
            //Immune: Ground
            if (move.type == TYPES::GROUND) {
                modifier = 0;
            }
            break;

        case TYPES::GHOST:
            //Weak: DARK, GHOST
            if (move.type == TYPES::DARK || move.type == TYPES::GHOST) {
                modifier = modifier * 2;
            }
            //Resist: BUG, POISON
            if (move.type == TYPES::BUG || move.type == TYPES::POISON) {
                modifier = modifier / 2;
            }
            //Immune: NORMAL, FIGHTING
            if (move.type == TYPES::NORMAL || move.type == TYPES::FIGHTING) {
                modifier = 0;
            }
            break;

        case TYPES::GRASS:
            //Weak: Bug, Fire, Flying, Ice, Poison
            if (move.type == TYPES::BUG || move.type == TYPES::FIRE || move.type == TYPES::FLYING ||
                move.type == TYPES::ICE || move.type == TYPES::POISON) {
                modifier = modifier * 2;
            }
            //Resist: Electric, Grass, Ground, Water
            if (move.type == TYPES::ELECTRIC || move.type == TYPES::GRASS || move.type == TYPES::GROUND || move.type == TYPES::WATER) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::GROUND:
            //Weak: WATER, GRASS, ICE
            if (move.type == TYPES::WATER || move.type == TYPES::GRASS || move.type == TYPES::ICE) {
                modifier = modifier * 2;
            }
            //Resist: POISON, ROCK
            if (move.type == TYPES::POISON || move.type == TYPES::ROCK) {
                modifier = modifier / 2;
            }
            //Immune: ELECTRIC
            if (move.type == TYPES::ELECTRIC) {
                modifier = 0;
            }
            break;

        case TYPES::ICE:
            //Weak: FIGHTING, ROCK, STEEL, FIRE
            if (move.type == TYPES::FIGHTING || move.type == TYPES::ROCK || move.type == TYPES::STEEL || move.type == TYPES::FIRE) {
                modifier = modifier * 2;
            }
            //Resist: ICE
            if (move.type == TYPES::ICE) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::NORMAL:
            //Weak: FIGHTING
            if (move.type == TYPES::FIGHTING) {
                modifier = modifier * 2;
            }
            //Immune: GHOST
            if (move.type == TYPES::GHOST) {
                modifier = 0;
            }
            break;

        case TYPES::POISON:
            //Weak: Ground, Psychic
            if (move.type == TYPES::GROUND || move.type == TYPES::PSYCHIC) {
                modifier = modifier * 2;
            }
            //Resist: Fighting, Poison, Bug, Grass, FAIRY
            if (move.type == TYPES::FIGHTING || move.type == TYPES::POISON || move.type == TYPES::BUG ||
                move.type == TYPES::GRASS || move.type == TYPES::FAIRY) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::PSYCHIC:
            //Weak: BUG, DARK, GHOST
            if (move.type == TYPES::FIGHTING || move.type == TYPES::POISON || move.type == TYPES::BUG) {
                modifier = modifier * 2;
            }
            //Resist: FIGHTING, PSYCHIC
            if (move.type == TYPES::FIGHTING || move.type == TYPES::PSYCHIC) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::ROCK:
            //Weak: FIGHTING, GROUND, STEEL, WATER, GRASS
            if (move.type == TYPES::FIGHTING || move.type == TYPES::GROUND || move.type == TYPES::STEEL ||
                move.type == TYPES::GRASS || move.type == TYPES::GRASS) {
                modifier = modifier * 2;
            }
            //Resist: NORMAL, FLYING, POISON, FIRE
            if (move.type == TYPES::NORMAL || move.type == TYPES::FLYING || move.type == TYPES::POISON || move.type == TYPES::FIRE) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::STEEL:
            //Weak: GROUND, FIGHTING, FIRE
            if (move.type == TYPES::GROUND || move.type == TYPES::FIGHTING || move.type == TYPES::FIRE) {
                modifier = modifier * 2;
            }
            //Resist: NORMAL, FLYING, ROCK, BUG, STEEL, GRASS, PSYCHIC, ICE, FAIRY, DRAGON
            if (move.type == TYPES::NORMAL || move.type == TYPES::FLYING || move.type == TYPES::ROCK || move.type == TYPES::BUG ||
                move.type == TYPES::STEEL || move.type == TYPES::GRASS || move.type == TYPES::PSYCHIC || move.type == TYPES::ICE ||
                move.type == TYPES::FAIRY || move.type == TYPES::DRAGON) {
                modifier = modifier / 2;
            }
            //Immune: POISON
            if (move.type == TYPES::POISON) {
                modifier = 0;
            }
            break;

        case TYPES::WATER:
            //Weak: Electric, Grass
            if (move.type == TYPES::ELECTRIC || move.type == TYPES::GRASS) {
                modifier = modifier * 2;
            }
            //Resist: Steel, Fire, Water, Ice
            if (move.type == TYPES::STEEL || move.type == TYPES::FIRE || move.type == TYPES::WATER || move.type == TYPES::ICE) {
                modifier = modifier / 2;
            }
            break;
        }
    }

    if (modifier > 1) {
        superEff++;
    }
    else if (modifier == 0) {
        noEff++;
    }
    else if (modifier < 1) {
        notVeryEff++;
    }

    damage = (int)(damage * modifier);
    dmgArray[0] = damage;
    dmgArray[1] = superEff;
    dmgArray[2] = notVeryEff;
    dmgArray[3] = noEff;  // Update as necessary for no effect cases
}

__device__ void damageCalcGPU(Pokemon& attacker, Pokemon& defender, Move move, int seed) {
    // Variables
    double critCalc = 0;
    double stab = 1.0;
    double randFact = 0;
    double force = 0;
    double object = 0;

    // Physical or special attack calculation
    if (move.category == 1) {
        force = attacker.statAtk[1];
        object = defender.statDef[1];
    }
    else if (move.category == 2) {
        force = attacker.statSpa[1];
        object = defender.statSpd[1];
    }

    // Critical hit calculation using random number generator
    int randVal = getRandom(seed, 0); // You can pass a custom seed and offset
    if (randVal < (attacker.statSpe[1] / 2)) {
        critCalc = 1.5;
    }
    else {
        critCalc = 1.0;
    }

    // Random factor calculation
    randFact = (randVal + 85) / 100.0;

    // Same Type Attack Bonus (STAB)
    if (move.type == attacker.type1 || move.type == attacker.type2) {
        stab = 1.5;
    }

    // Base damage calculation
    int baseDamage = static_cast<int>((((22) * move.power * (force / object)) / (50.0 + 2)) * critCalc * randFact * stab);

    // Get type multiplier
    typeMultiplierGPU(baseDamage, move, defender);
    baseDamage = dmgArray[0];

    // Apply damage to defender's HP
    defender.healthPoints -= baseDamage;
}

__device__ void statusCalcGPU(Pokemon& attacker, Pokemon& defender, Move move) {
    // {buff/debuff, atk, def, spa, spd, spe, acc, eva}
    int* effect = move.statEffect;
    double mult;
    int multStage;

    if (effect[0] == 0) { // If buffing self
        if (effect[1] != 0) { // Buff attack
            multStage = effect[1];
            attacker.statAtk[0] += multStage;
            if (attacker.statAtk[0] > 6) {
                attacker.statAtk[0] = 6;
            }
            else if (attacker.statAtk[0] < -6) {
                attacker.statAtk[0] = -6;
            }
            multStage = attacker.statAtk[0];
            mult = statMultiplierGPU[multStage];
            int newAtk = static_cast<int>(mult * attacker.statAtk[1]);
            attacker.statAtk[1] = newAtk;
            /*std::cout << attacker.getPokeName() << "'s attack rose!" << std::endl;*/
        }
        if (effect[2] != 0) { // Buff defense
            multStage = effect[2];
            attacker.statDef[0] += multStage;
            if (attacker.statDef[0] > 6) {
                attacker.statDef[0] = 6;
            }
            else if (attacker.statDef[0] < -6) {
                attacker.statDef[0] = -6;
            }
            multStage = attacker.statDef[0];
            mult = statMultiplierGPU[multStage];
            int newDef = static_cast<int>(mult * attacker.statDef[1]);
            attacker.statDef[1] = newDef;
            //std::cout << attacker.getPokeName() << "'s defense rose!" << std::endl;
        }
        if (effect[3] != 0) { // Buff special attack
            multStage = effect[3];
            attacker.statSpa[0] += multStage;
            if (attacker.statSpa[0] > 6) {
                attacker.statSpa[0] = 6;
            }
            else if (attacker.statSpa[0] < -6) {
                attacker.statSpa[0] = -6;
            }
            multStage = attacker.statSpa[0];
            mult = statMultiplierGPU[multStage];
            int newSpa = static_cast<int>(mult * attacker.statSpa[1]);
            attacker.statSpa[1] = newSpa;
            //std::cout << attacker.getPokeName() << "'s special attack rose!" << std::endl;
        }
        if (effect[4] != 0) { // Buff special defense
            multStage = effect[4];
            attacker.statSpd[0] += multStage;
            if (attacker.statSpd[0] > 6) {
                attacker.statSpd[0] = 6;
            }
            else if (attacker.statSpd[0] < -6) {
                attacker.statSpd[0] = -6;
            }
            multStage = attacker.statSpd[0];
            mult = statMultiplierGPU[multStage];
            int newSpd = static_cast<int>(mult * attacker.statSpd[1]);
            attacker.statSpd[1] = newSpd;
            //std::cout << attacker.getPokeName() << "'s special defense rose!" << std::endl;
        }
        if (effect[5] != 0) { // Buff speed
            multStage = effect[5];
            attacker.statSpe[0] += multStage;
            if (attacker.statSpe[0] > 6) {
                attacker.statSpe[0] = 6;
            }
            else if (attacker.statSpe[0] < -6) {
                attacker.statSpe[0] = -6;
            }
            multStage = attacker.statSpe[0];
            mult = statMultiplierGPU[multStage];
            int newSpe = static_cast<int>(mult * attacker.statSpe[1]);
            attacker.statSpe[1] = newSpe;
            //std::cout << attacker.getPokeName() << "'s speed rose!" << std::endl;
        }
        if (effect[6] != 0) { // Buff accuracy
            //std::cout << attacker.getPokeName() << "'s accuracy rose!" << std::endl;
        }
        if (effect[7] != 0) { // Buff evasion
            //std::cout << attacker.getPokeName() << "'s evasion rose!" << std::endl;
        }
    }
    else if (effect[0] == 1) { // Debuffing opponent
        if (effect[1] != 0) { // Debuff attack
            multStage = effect[1];
            defender.statAtk[0] += multStage;
            if (defender.statAtk[0] > 6) {
                defender.statAtk[0] = 6;
            }
            else if (defender.statAtk[0] < -6) {
                defender.statAtk[0] = -6;
            }
            multStage = defender.statAtk[0];
            mult = statMultiplierGPU[multStage];
            int newAtk = static_cast<int>(mult * defender.statAtk[1]);
            defender.statAtk[1] = newAtk;
            //std::cout << defender.getPokeName() << "'s attack fell!" << std::endl;
        }
        if (effect[2] != 0) { // Debuff defense
            multStage = effect[2];
            defender.statDef[0] += multStage;
            if (defender.statDef[0] > 6) {
                defender.statDef[0] = 6;
            }
            else if (defender.statDef[0] < -6) {
                defender.statDef[0] = -6;
            }
            multStage = defender.statDef[0];
            mult = statMultiplierGPU[multStage];
            int newDef = static_cast<int>(mult * defender.statDef[1]);
            defender.statDef[1] = newDef;
            //std::cout << defender.getPokeName() << "'s defense fell!" << std::endl;
        }
        if (effect[3] != 0) { // Debuff special attack
            multStage = effect[3];
            defender.statSpa[0] += multStage;
            if (defender.statSpa[0] > 6) {
                defender.statSpa[0] = 6;
            }
            else if (defender.statSpa[0] < -6) {
                defender.statSpa[0] = -6;
            }
            multStage = defender.statSpa[0];
            mult = statMultiplierGPU[multStage];
            int newSpa = static_cast<int>(mult * defender.statSpa[1]);
            defender.statSpa[1] = newSpa;
            //std::cout << defender.getPokeName() << "'s special attack fell!" << std::endl;
        }
        if (effect[4] != 0) { // Debuff special defense
            multStage = effect[4];
            defender.statSpd[0] += multStage;
            if (defender.statSpd[0] > 6) {
                defender.statSpd[0] = 6;
            }
            else if (defender.statSpd[0] < -6) {
                defender.statSpd[0] = -6;
            }
            multStage = defender.statSpd[0];
            mult = statMultiplierGPU[multStage];
            int newSpd = static_cast<int>(mult * defender.statSpd[1]);
            defender.statSpd[1] = newSpd;
            //std::cout << defender.getPokeName() << "'s special defense fell!" << std::endl;
        }
        if (effect[5] != 0) { // Debuff speed
            multStage = effect[5];
            defender.statSpe[0] += multStage;
            if (defender.statSpe[0] > 6) {
                defender.statSpe[0] = 6;
            }
            else if (defender.statSpe[0] < -6) {
                defender.statSpe[0] = -6;
            }
            multStage = defender.statSpe[0];
            mult = statMultiplierGPU[multStage];
            int newSpe = static_cast<int>(mult * defender.statSpe[1]);
            defender.statSpe[1] = newSpe;
            //std::cout << defender.getPokeName() << "'s speed fell!" << std::endl;
        }
        if (effect[6] != 0) { // Debuff accuracy
            //std::cout << defender.getPokeName() << "'s accuracy fell!" << std::endl;
        }
        if (effect[7] != 0) { // Debuff evasion
            //std::cout << defender.getPokeName() << "'s evasion fell!" << std::endl;
        }
    }
}

__device__ void battleGPU(Pokemon& pokemon1, Pokemon& pokemon2, Pokemon& winner, int seed) {
    int moveIndex;
    Move selected = pokemon1.moves[0]; // initial move bc I don't want to figure out the actual problem

    while (pokemon1.healthPoints >= 1 && pokemon2.healthPoints >= 1) {
        if (pokemon1.statSpe[1] > pokemon2.statSpe[1]) {
            moveIndex = getRandom(seed, 1) % sizeof(pokemon1.moves) / sizeof(pokemon1.moves[0]);
            selected = pokemon1.moves[moveIndex];

            if (selected.category == 3) {
                statusCalcGPU(pokemon1, pokemon2, selected);
            }
            else {
                damageCalcGPU(pokemon1, pokemon2, selected, seed);
            }

            if (pokemon2.healthPoints < 1) break;

            moveIndex = getRandom(seed, 2) % sizeof(pokemon2.moves) / sizeof(pokemon2.moves[0]);
            selected = pokemon2.moves[moveIndex];

            if (selected.category == 3) {
                statusCalcGPU(pokemon2, pokemon1, selected);
            }
            else {
                damageCalcGPU(pokemon2, pokemon1, selected, seed);
            }
        }
        else {
            moveIndex = getRandom(seed, 3) % sizeof(pokemon2.moves) / sizeof(pokemon2.moves[0]);
            selected = pokemon2.moves[moveIndex];

            if (selected.category == 3) {
                statusCalcGPU(pokemon2, pokemon1, selected);
            }
            else {
                damageCalcGPU(pokemon2, pokemon1, selected, seed);
            }

            if (pokemon1.healthPoints < 1) break;

            moveIndex = getRandom(seed, 4) % sizeof(pokemon1.moves) / sizeof(pokemon1.moves[0]);
            selected = pokemon1.moves[moveIndex];

            if (selected.category == 3) {
                statusCalcGPU(pokemon1, pokemon2, selected);
            }
            else {
                damageCalcGPU(pokemon1, pokemon2, selected, seed);
            }
        }
    }

    if (pokemon2.healthPoints < 1) {
        winner = pokemon1;
    }
    else {
        winner = pokemon2;
    }
}


// Kernel to simulate many battles
__global__ void battleKernel(Pokemon* p1Array, Pokemon* p2Array, Pokemon* results, int numBattles) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < numBattles) {
        battleGPU(p1Array[idx], p2Array[idx], results[idx], idx + 1234); // Unique seed per thread
    }
}

bool pokeBattleGPU(Pokemon* pokemon1, Pokemon* pokemon2, Pokemon* results, int NUM_POKEMON) {
    cudaError_t status;
    Pokemon* d_p1, * d_p2;
    Pokemon* r1;

    // Allocate memory on the device
    cudaMalloc((void**)&d_p1, sizeof(Pokemon) * NUM_POKEMON);
    cudaMalloc((void**)&d_p2, sizeof(Pokemon) * NUM_POKEMON);
    cudaMalloc((void**)&r1, sizeof(Pokemon) * NUM_POKEMON);

    // Copy input Pokémon from host to device
    cudaMemcpy(d_p1, &pokemon1, sizeof(Pokemon), cudaMemcpyHostToDevice);
    cudaMemcpy(d_p2, &pokemon2, sizeof(Pokemon), cudaMemcpyHostToDevice);

    dim3 blockSize(TILE_SIZE);
    dim3 gridSize((NUM_POKEMON + TILE_SIZE - 1) / TILE_SIZE);

    // Launch 1 thread to simulate 1 battle
    battleKernel << <gridSize, blockSize >> > (d_p1, d_p2, results, NUM_POKEMON);
    cudaDeviceSynchronize(); // Wait for GPU to finish

    status = cudaGetLastError();
    if (status != cudaSuccess) {
        std::cerr << "Kernel failed: " << cudaGetErrorString(status) << std::endl;
        cudaFree(d_p1);
        cudaFree(d_p2);
        cudaFree(r1);
        return false;
    }

    cudaMemcpy(&results, r1, sizeof(Pokemon), cudaMemcpyDeviceToHost);

    // Free GPU memory
    cudaFree(d_p1);
    cudaFree(d_p2);
    cudaFree(r1);

    return true;
}