#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <curand_kernel.h>
#include <iostream>
#include <unordered_map>
#include <random>
#include <string>
#include "Move.h"
#include "TYPES.h"
#include "Pokemon.h"
#include "PokeDex.h"
#include "AttackDex.h"
#include "PokemonData.h"
#include "Slowdown.h"
#include <vector>
#include <random>
#include <thread>
#include <chrono>

#ifndef COMMON_H
#include "common.h"
#endif

const int TILE_SIZE = 256;

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

//__constant__ double evaMultiplierGPU[13] = {
//    9.0 / 3.0, 8.0 / 3.0, 7.0 / 3.0,
//    6.0 / 3.0, 5.0 / 3.0, 4.0 / 3.0,
//    3.0 / 3.0,
//    3.0 / 4.0, 3.0 / 5.0, 3.0 / 6.0,
//    3.0 / 7.0, 3.0 / 8.0, 3.0 / 9.0
//};

__device__ static void typeMultiplierGPU(int damage, Move move, TYPES defenderType1, TYPES defenderType2, int* outArray) {
    double modifier = 1.0;
    int superEff = 0;
    int notVeryEff = 0;
    int noEff = 0;

    // Check if defender has a primary type
    if (defenderType1 != TYPES::NONE) { // Adjust as necessary to handle null or no type
        switch (defenderType1) {
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

    if (defenderType2 != TYPES::NONE) { // Adjust as necessary to handle null or no type
        switch (defenderType2) {
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
    outArray[0] = damage;
    outArray[1] = superEff;
    outArray[2] = notVeryEff;
    outArray[3] = noEff;  // Update as necessary for no effect cases
}

// GPU-compatible random number generation
__device__ void initRandom(curandState* state, int seed, int idx) {
    curand_init(seed, idx, 0, state);
}

__device__ int getRandom(curandState* state) {
    return curand(state) % 100;
}

__device__ static int targetHitGPU(int attackerAccST, int defenderEvaST, Move move, curandState* rng) {
    int accMove = move.accuracy;

    int stage = static_cast<int>(attackerAccST) - (defenderEvaST);
    if (stage > 6) stage = 6;
    else if (stage < -6) stage = -6;
    double stageMultiplier = accMultiplierGPU[stage];

    int r = curand(rng) % 100 + 1;

    if (r <= (accMove * stageMultiplier)) {
        return 1; // Hit
    }
    else {
        return 0; // Miss
    }
}

__device__ void damageCalcGPU(
    int attackerAtk, int defenderDef,
    int attackerSpa, int defenderSpd,
    int attackerSpe,
    TYPES attackerType1, TYPES attackerType2,
    TYPES defenderType1, TYPES defenderType2,
    int& defenderHP,
    Move move,
    int attackerAccST, int defenderEvaST,
    curandState* rng
) {
    double critMult = 1.0;
    double stab = 1.0;
    double randFact = 1.0;
    double force = 0.0;
    double object = 0.0;

    if (!targetHitGPU(attackerAccST, defenderEvaST, move, rng)) {
        return; // Move missed
    }

    // Choose attacking and defending stats based on move category
    if (move.category == 1) { // Physical
        force = attackerAtk;
        object = defenderDef;
    }
    else if (move.category == 2) { // Special
        force = attackerSpa;
        object = defenderSpd;
    }

    // Critical hit calculation
    int randVal = curand(rng) % 256;
    if (randVal < (attackerSpe / 2)) {
        critMult = 1.5;
    }

    // Random damage variance (85% - 100%)
    randFact = (randVal % 16 + 85) / 100.0;

    // Same Type Attack Bonus
    if (move.type == attackerType1 || move.type == attackerType2) {
        stab = 1.5;
    }

    // Base damage formula
    int baseDamage = static_cast<int>(
        (((22.0 * move.power * (force / object)) / (50.0 + 2))
            * critMult * randFact * stab)
        );

    // Apply type effectiveness
    int dmgArray[4] = {};
    typeMultiplierGPU(baseDamage, move, defenderType1, defenderType2, dmgArray);
    baseDamage = dmgArray[0];
    if (baseDamage < 1) baseDamage = 1;

    // Inflict damage
    defenderHP -= baseDamage;
}

__device__ void statusCalcGPU(
    int attackerAtkST, int defenderAtkST,
    int attackerDefST, int defenderDefST,
    int attackerSpaST, int defenderSpaST,
    int attackerSpdST, int defenderSpdST,
    int attackerSpeST, int defenderSpeST,
    int attackerAccST, int defenderAccST,
    int attackerEvaST, int defenderEvaST,
    int attackerAtk, int defenderAtk,
    int attackerDef, int defenderDef,
    int attackerSpa, int defenderSpa,
    int attackerSpd, int defenderSpd,
    int attackerSpe, int defenderSpe, Move move
) {
    // {buff/debuff, atk, def, spa, spd, spe, acc, eva}
    int* effect = move.statEffect;
    double mult;
    int multStage;

    if (effect[0] == 0) { // If buffing self
        if (effect[1] != 0) { // Buff attack
            multStage = effect[1];
            attackerAtkST += multStage;
            if (attackerAtkST > 6) {
                attackerAtkST = 6;
            }
            else if (attackerAtkST < -6) {
                attackerAtkST = -6;
            }
            multStage = attackerAtkST;
            mult = statMultiplierGPU[multStage];
            int newAtk = static_cast<int>(mult * attackerAtk);
            attackerAtk = newAtk;
            /*std::cout << attacker.getPokeName() << "'s attack rose!" << std::endl;*/
        }
        if (effect[2] != 0) { // Buff defense
            multStage = effect[2];
            attackerDefST += multStage;
            if (attackerDefST > 6) {
                attackerDefST = 6;
            }
            else if (attackerDefST < -6) {
                attackerDefST = -6;
            }
            multStage = attackerDefST;
            mult = statMultiplierGPU[multStage];
            int newDef = static_cast<int>(mult * attackerDef);
            attackerDef = newDef;
            //std::cout << attacker.getPokeName() << "'s defense rose!" << std::endl;
        }
        if (effect[3] != 0) { // Buff special attack
            multStage = effect[3];
            attackerSpaST += multStage;
            if (attackerSpaST > 6) {
                attackerSpaST = 6;
            }
            else if (attackerSpaST < -6) {
                attackerSpaST = -6;
            }
            multStage = attackerSpaST;
            mult = statMultiplierGPU[multStage];
            int newSpa = static_cast<int>(mult * attackerSpa);
            attackerSpa = newSpa;
            //std::cout << attacker.getPokeName() << "'s special attack rose!" << std::endl;
        }
        if (effect[4] != 0) { // Buff special defense
            multStage = effect[4];
            attackerSpdST += multStage;
            if (attackerSpdST > 6) {
                attackerSpdST = 6;
            }
            else if (attackerSpdST < -6) {
                attackerSpdST = -6;
            }
            multStage = attackerSpdST;
            mult = statMultiplierGPU[multStage];
            int newSpd = static_cast<int>(mult * attackerSpd);
            attackerSpd = newSpd;
            //std::cout << attacker.getPokeName() << "'s special defense rose!" << std::endl;
        }
        if (effect[5] != 0) { // Buff speed
            multStage = effect[5];
            attackerSpeST += multStage;
            if (attackerSpeST > 6) {
                attackerSpeST = 6;
            }
            else if (attackerSpeST < -6) {
                attackerSpeST = -6;
            }
            multStage = attackerSpeST;
            mult = statMultiplierGPU[multStage];
            int newSpe = static_cast<int>(mult * attackerSpeST);
            attackerSpeST = newSpe;
            //std::cout << attacker.getPokeName() << "'s speed rose!" << std::endl;
        }
        if (effect[6] != 0) { // Buff accuracy
            multStage = effect[6];
            attackerAccST += multStage;
            if (attackerAccST > 6) {
                attackerAccST = 6;
            }
            else if (attackerAccST < -6) {
                attackerAccST = -6;
            }
            //std::cout << attacker.getPokeName() << "'s accuracy rose!" << std::endl;
        }
        if (effect[7] != 0) { // Buff evasion
            multStage = effect[7];
            attackerEvaST += multStage;
            if (attackerEvaST > 6) {
                attackerEvaST = 6;
            }
            else if (attackerEvaST < -6) {
                attackerEvaST = -6;
            }
            //std::cout << attacker.getPokeName() << "'s evasion rose!" << std::endl;
        }
    }
    else if (effect[0] == 1) { // Debuffing opponent
        if (effect[1] != 0) { // Debuff attack
            multStage = effect[1];
            defenderAtkST += multStage;
            if (defenderAtkST > 6) {
                defenderAtkST = 6;
            }
            else if (defenderAtkST < -6) {
                defenderAtkST = -6;
            }
            multStage = defenderAtkST;
            mult = statMultiplierGPU[multStage];
            int newAtk = static_cast<int>(mult * defenderAtk);
            defenderAtk = newAtk;
            //std::cout << defender.getPokeName() << "'s attack fell!" << std::endl;
        }
        if (effect[2] != 0) { // Debuff defense
            multStage = effect[2];
            defenderDefST += multStage;
            if (defenderDefST > 6) {
                defenderDefST = 6;
            }
            else if (defenderDefST < -6) {
                defenderDefST = -6;
            }
            multStage = defenderDefST;
            mult = statMultiplierGPU[multStage];
            int newDef = static_cast<int>(mult * defenderDef);
            defenderDef = newDef;
            //std::cout << defender.getPokeName() << "'s defense fell!" << std::endl;
        }
        if (effect[3] != 0) { // Debuff special attack
            multStage = effect[3];
            defenderSpaST += multStage;
            if (defenderSpaST > 6) {
                defenderSpaST = 6;
            }
            else if (defenderSpaST < -6) {
                defenderSpaST = -6;
            }
            multStage = defenderSpaST;
            mult = statMultiplierGPU[multStage];
            int newSpa = static_cast<int>(mult * defenderSpa);
            defenderSpa = newSpa;
            //std::cout << defender.getPokeName() << "'s special attack fell!" << std::endl;
        }
        if (effect[4] != 0) { // Debuff special defense
            multStage = effect[4];
            defenderSpdST += multStage;
            if (defenderSpdST > 6) {
                defenderSpdST = 6;
            }
            else if (defenderSpdST < -6) {
                defenderSpdST = -6;
            }
            multStage = defenderSpdST;
            mult = statMultiplierGPU[multStage];
            int newSpd = static_cast<int>(mult * defenderSpd);
            defenderSpd = newSpd;
            //std::cout << defender.getPokeName() << "'s special defense fell!" << std::endl;
        }
        if (effect[5] != 0) { // Debuff speed
            multStage = effect[5];
            defenderSpeST += multStage;
            if (defenderSpeST > 6) {
                defenderSpeST = 6;
            }
            else if (defenderSpeST < -6) {
                defenderSpeST = -6;
            }
            multStage = defenderSpeST;
            mult = statMultiplierGPU[multStage];
            int newSpe = static_cast<int>(mult * defenderSpeST);
            defenderSpeST = newSpe;
            //std::cout << defender.getPokeName() << "'s speed fell!" << std::endl;
        }
        if (effect[6] != 0) { // Debuff accuracy
            multStage = effect[6];
            defenderAccST += multStage;
            if (defenderAccST > 6) {
                defenderAccST = 6;
            }
            else if (defenderAccST < -6) {
                defenderAccST = -6;
            }
            //std::cout << defender.getPokeName() << "'s accuracy fell!" << std::endl;
        }
        if (effect[7] != 0) { // Debuff evasion
            multStage = effect[7];
            defenderEvaST += multStage;
            if (defenderEvaST > 6) {
                defenderEvaST = 6;
            }
            else if (defenderEvaST < -6) {
                defenderEvaST = -6;
            }
            //std::cout << defender.getPokeName() << "'s evasion fell!" << std::endl;
        }
    }
}

__device__ void battleGPUNew(PokemonData* p1, PokemonData* p2, int* result, int idx, curandState* rng) {
    int canUse = 0;
    while (p1->healthPoints[idx] > 0 && p2->healthPoints[idx] > 0) {
        Move selected = p1->moves[0][0]; //placeholder for initialization
        if (p1->speed[idx] > p2->speed[idx]) {
            while (!canUse) {
                selected = p1->moves[idx][curand(rng) % 4];
                if (selected.pp < 1) {
                    //do nothing i.e. try for another move
                }
                else {
                    selected.pp -=1;
                    canUse++;
                }
            }
            canUse--;

            if (selected.category == 3) {
                statusCalcGPU(
                    p1->statAtk[idx][0], p2->statAtk[idx][0],
                    p1->statDef[idx][0], p2->statDef[idx][0],
                    p1->statSpa[idx][0], p2->statSpa[idx][0],
                    p1->statSpd[idx][0], p2->statSpd[idx][0],
                    p1->statSpe[idx][0], p2->statSpe[idx][0],
                    p1->statAcc[idx][0], p2->statAcc[idx][0],
                    p1->statEva[idx][0], p2->statEva[idx][0],
                    p1->statAtk[idx][1], p2->statAtk[idx][1],
                    p1->statDef[idx][1], p2->statDef[idx][1],
                    p1->statSpa[idx][1], p2->statSpa[idx][1],
                    p1->statSpd[idx][1], p2->statSpd[idx][1],
                    p1->statSpe[idx][1], p2->statSpe[idx][1],
                    selected
                );
            }
            else {
                damageCalcGPU(
                    p1->attack[idx], p2->defense[idx],
                    p1->specialAttack[idx], p2->specialDefense[idx],
                    p1->speed[idx],
                    p1->type1[idx], p1->type2[idx],
                    p2->type1[idx], p2->type2[idx],
                    p2->healthPoints[idx],
                    selected,
                    p1->statAcc[idx][0], p2->statEva[idx][0],
                    rng
                );
            }

            if (p2->healthPoints[idx] < 1) break;

            while (!canUse) {
                selected = p2->moves[idx][curand(rng) % 4];
                if (selected.pp < 1) {
                    //do nothing i.e. try for another move
                }
                else {
                    selected.pp -=1;
                    canUse++;
                }
            }
            canUse--;

            if (selected.category == 3) {
                statusCalcGPU(
                    p2->statAtk[idx][0], p1->statAtk[idx][0],
                    p2->statDef[idx][0], p1->statDef[idx][0],
                    p2->statSpa[idx][0], p1->statSpa[idx][0],
                    p2->statSpd[idx][0], p1->statSpd[idx][0],
                    p2->statSpe[idx][0], p1->statSpe[idx][0],
                    p2->statAcc[idx][0], p1->statAcc[idx][0],
                    p2->statEva[idx][0], p1->statEva[idx][0],
                    p2->statAtk[idx][1], p1->statAtk[idx][1],
                    p2->statDef[idx][1], p1->statDef[idx][1],
                    p2->statSpa[idx][1], p1->statSpa[idx][1],
                    p2->statSpd[idx][1], p1->statSpd[idx][1],
                    p2->statSpe[idx][1], p1->statSpe[idx][1],
                    selected
                );
            }
            else {
                damageCalcGPU(
                    p2->attack[idx], p1->defense[idx],
                    p2->specialAttack[idx], p1->specialDefense[idx],
                    p2->speed[idx],
                    p2->type1[idx], p2->type2[idx],
                    p1->type1[idx], p1->type2[idx],
                    p1->healthPoints[idx],
                    selected,
                    p2->statAcc[idx][0], p1->statEva[idx][0],
                    rng
                );
            }

        }
        else {
            while (!canUse) {
                selected = p2->moves[idx][curand(rng) % 4];
                if (selected.pp < 1) {
                    //do nothing i.e. try for another move
                }
                else {
                    selected.pp -=1;
                    canUse++;
                }
            }
            canUse--;

            if (selected.category == 3) {
                statusCalcGPU(
                    p2->statAtk[idx][0], p1->statAtk[idx][0],
                    p2->statDef[idx][0], p1->statDef[idx][0],
                    p2->statSpa[idx][0], p1->statSpa[idx][0],
                    p2->statSpd[idx][0], p1->statSpd[idx][0],
                    p2->statSpe[idx][0], p1->statSpe[idx][0],
                    p2->statAcc[idx][0], p1->statAcc[idx][0],
                    p2->statEva[idx][0], p1->statEva[idx][0],
                    p2->statAtk[idx][1], p1->statAtk[idx][1],
                    p2->statDef[idx][1], p1->statDef[idx][1],
                    p2->statSpa[idx][1], p1->statSpa[idx][1],
                    p2->statSpd[idx][1], p1->statSpd[idx][1],
                    p2->statSpe[idx][1], p1->statSpe[idx][1],
                    selected
                );
            }
            else {
                damageCalcGPU(
                    p2->attack[idx], p1->defense[idx],
                    p2->specialAttack[idx], p1->specialDefense[idx],
                    p2->speed[idx],
                    p2->type1[idx], p2->type2[idx],
                    p1->type1[idx], p1->type2[idx],
                    p1->healthPoints[idx],
                    selected,
                    p2->statAcc[idx][0], p1->statEva[idx][0],
                    rng
                );
            }

            if (p1->healthPoints[idx] < 1) break;

            while (!canUse) {
                selected = p1->moves[idx][curand(rng) % 4];
                if (selected.pp < 1) {
                    //do nothing i.e. try for another move
                }
                else {
                    selected.pp -=1;
                    canUse++;
                }
            }
            canUse--;

            if (selected.category == 3) {
                statusCalcGPU(
                    p1->statAtk[idx][0], p2->statAtk[idx][0],
                    p1->statDef[idx][0], p2->statDef[idx][0],
                    p1->statSpa[idx][0], p2->statSpa[idx][0],
                    p1->statSpd[idx][0], p2->statSpd[idx][0],
                    p1->statSpe[idx][0], p2->statSpe[idx][0],
                    p1->statAcc[idx][0], p2->statAcc[idx][0],
                    p1->statEva[idx][0], p2->statEva[idx][0],
                    p1->statAtk[idx][1], p2->statAtk[idx][1],
                    p1->statDef[idx][1], p2->statDef[idx][1],
                    p1->statSpa[idx][1], p2->statSpa[idx][1],
                    p1->statSpd[idx][1], p2->statSpd[idx][1],
                    p1->statSpe[idx][1], p2->statSpe[idx][1],
                    selected
                );
            }
            else {
                damageCalcGPU(
                    p1->attack[idx], p2->defense[idx],
                    p1->specialAttack[idx], p2->specialDefense[idx],
                    p1->speed[idx],
                    p1->type1[idx], p1->type2[idx],
                    p2->type1[idx], p2->type2[idx],
                    p2->healthPoints[idx],
                    selected,
                    p1->statAcc[idx][0], p2->statEva[idx][0],
                    rng
                );
            }

        }
    }

    // Write the result to the result array
    if (p2->healthPoints[idx] < 1) {
        result[idx] = 1;
    }
    else {
        result[idx] = 2;
    }
}

__global__ void battleKernel(PokemonData* p1, PokemonData* p2, int* result, int numBattles) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < numBattles) {
        curandState rng;
        curand_init(12345, idx, 0, &rng);
        battleGPUNew(p1, p2, result, idx, &rng);  // Pass pointers
    }
}

bool pokeBattleGPUNew(PokemonData* p1, PokemonData* p2, int* result, int numBattles) {
    PokemonData* d_p1;
    PokemonData* d_p2;
    int* d_result;

    cudaMalloc((void**)&d_p1, sizeof(PokemonData));
    cudaMalloc((void**)&d_p2, sizeof(PokemonData));
    cudaMalloc((void**)&d_result, sizeof(int) * numBattles);

    // Copy host PokemonData (single structs, each holding SoA arrays)
    cudaMemcpy(d_p1, p1, sizeof(PokemonData), cudaMemcpyHostToDevice);
    cudaMemcpy(d_p2, p2, sizeof(PokemonData), cudaMemcpyHostToDevice);

    dim3 blockSize(TILE_SIZE);
    dim3 gridSize((numBattles + TILE_SIZE - 1) / TILE_SIZE);

    // Launch the kernel
    battleKernel << <gridSize, blockSize >> > (d_p1, d_p2, d_result, numBattles);

    // Check for errors
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Launch Error: " << cudaGetErrorString(err) << std::endl;
        return false;
    }

    // Copy results back
    cudaMemcpy(result, d_result, sizeof(int) * numBattles, cudaMemcpyDeviceToHost);

    // Print results

    cudaFree(d_p1);
    cudaFree(d_p2);
    cudaFree(d_result);

    return true;
}

