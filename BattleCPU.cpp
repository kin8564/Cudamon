#include <math.h>
#include <iostream> // cout, stream

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

static const std::unordered_map<int, double> statMultiplier = {
    {-6, 2.0 / 8.0}, {-5, 2.0 / 7.0}, {-4, 2.0 / 6.0},
    {-3, 2.0 / 5.0}, {-2, 2.0 / 4.0}, {-1, 2.0 / 3.0},
    {0,  2.0 / 2.0}, {1,  3.0 / 2.0}, {2,  4.0 / 2.0},
    {3,  5.0 / 2.0}, {4,  6.0 / 2.0}, {5,  7.0 / 2.0},
    {6,  8.0 / 2.0}
};

static const std::unordered_map<int, double> accMultiplier = {
    {-6, 3.0 / 9.0}, {-5, 3.0 / 8.0}, {-4, 3.0 / 7.0},
    {-3, 3.0 / 6.0}, {-2, 3.0 / 5.0}, {-1, 3.0 / 4.0},
    {0,  3.0 / 3.0}, {1,  4.0 / 3.0}, {2,  5.0 / 3.0},
    {3,  6.0 / 3.0}, {4,  7.0 / 3.0}, {5,  8.0 / 3.0},
    {6,  9.0 / 3.0}
};

//static const std::unordered_map<int, double> evaMultiplier = {
//    {6,  3.0 / 9.0}, {5,  3.0 / 8.0}, {4,  3.0 / 7.0},
//    {3,  3.0 / 6.0}, {2,  3.0 / 5.0}, {1,  3.0 / 4.0},
//    {0,  3.0 / 3.0}, {-1, 4.0 / 3.0}, {-2, 5.0 / 3.0},
//    {-3, 6.0 / 3.0}, {-4, 7.0 / 3.0}, {-5, 8.0 / 3.0},
//    {-6, 9.0 / 3.0}
//};

static std::vector<int> typeMultiplier(int damage, Move move, Pokemon defender) {
    double modifier = 1.0;
    int superEff = 0;
    int notVeryEff = 0;
    int noEff = 0;

    std::vector<int> dmgArray(4, 0);

    // Check if defender has a primary type
    if (defender.getTypeA() != TYPES::NONE) { // Adjust as necessary to handle null or no type
        switch (defender.getTypeA()) {
        case TYPES::BUG:
            // Weak: Flying, Rock, Fire
            if (move.getType() == TYPES::FLYING || move.getType() == TYPES::ROCK || move.getType() == TYPES::FIRE) {
                modifier *= 2;
            }
            // Resist: Fighting, Ground, Grass
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::GROUND || move.getType() == TYPES::GRASS) {
                modifier /= 2;
            }
            break;

        case TYPES::DARK:
            //Weak: BUG, FAIRY, FIGHTING
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::FAIRY || move.getType() == TYPES::FIGHTING) {
                modifier = modifier * 2;
            }
            //Resist: GHOST, DARK
            if (move.getType() == TYPES::GHOST || move.getType() == TYPES::DARK) {
                modifier = modifier / 2;
            }
            //Immune: Psychic
            if (move.getType() == TYPES::PSYCHIC) {
                modifier = 0;
            }
            break;

        case TYPES::DRAGON:
            //Weak: DRAGON, FAIRY, ICE
            if (move.getType() == TYPES::DRAGON || move.getType() == TYPES::FAIRY || move.getType() == TYPES::ICE) {
                modifier = modifier * 2;
            }
            //Resist: FIRE, WATER, GRASS, ELECTRIC
            if (move.getType() == TYPES::FIRE || move.getType() == TYPES::WATER || move.getType() == TYPES::GRASS || move.getType() == TYPES::ELECTRIC) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::ELECTRIC:
            //Weak: GROUND
            if (move.getType() == TYPES::GROUND) {
                modifier = modifier * 2;
            }
            //Resist: FLYING, ELECTRIC, STEEL
            if (move.getType() == TYPES::FLYING || move.getType() == TYPES::ELECTRIC || move.getType() == TYPES::STEEL) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FAIRY:
            //Weak: POISON, STEEL
            if (move.getType() == TYPES::POISON || move.getType() == TYPES::STEEL) {
                modifier = modifier * 2;
            }
            //Resist: BUG, DARK, FIGHTING
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::DARK || move.getType() == TYPES::FIGHTING) {
                modifier = modifier / 2;
            }
            //Immune: DRAGON
            if (move.getType() == TYPES::DRAGON) {
                modifier = 0;
            }
            break;

        case TYPES::FIGHTING:
            //Weak: FAIRY, FLYING, PSYCHIC
            if (move.getType() == TYPES::FAIRY || move.getType() == TYPES::FLYING || move.getType() == TYPES::PSYCHIC) {
                modifier = modifier * 2;
            }
            //Resist: BUG, DARK, ROCK
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::DARK || move.getType() == TYPES::ROCK) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FIRE:
            //Weak: Ground, Rock, Water
            if (move.getType() == TYPES::GROUND || move.getType() == TYPES::ROCK || move.getType() == TYPES::WATER) {
                modifier = modifier * 2;
            }
            //Resist: Bug, Steel, Fire, Grass, Ice, Fairy
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::STEEL || move.getType() == TYPES::FIRE ||
                move.getType() == TYPES::GRASS || move.getType() == TYPES::ICE || move.getType() == TYPES::FAIRY) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FLYING:
            //Weak: Electric, Ice, Rock
            if (move.getType() == TYPES::ELECTRIC || move.getType() == TYPES::ICE || move.getType() == TYPES::ROCK) {
                modifier = modifier * 2;
            }
            //Resist: Bug, Fighting, Grass
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::FIGHTING || move.getType() == TYPES::GRASS) {
                modifier = modifier / 2;
            }
            //Immune: Ground
            if (move.getType() == TYPES::GROUND) {
                modifier = 0;
            }
            break;

        case TYPES::GHOST:
            //Weak: DARK, GHOST
            if (move.getType() == TYPES::DARK || move.getType() == TYPES::GHOST) {
                modifier = modifier * 2;
            }
            //Resist: BUG, POISON
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::POISON) {
                modifier = modifier / 2;
            }
            //Immune: NORMAL, FIGHTING
            if (move.getType() == TYPES::NORMAL || move.getType() == TYPES::FIGHTING) {
                modifier = 0;
            }
            break;

        case TYPES::GRASS:
            //Weak: Bug, Fire, Flying, Ice, Poison
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::FIRE || move.getType() == TYPES::FLYING ||
                move.getType() == TYPES::ICE || move.getType() == TYPES::POISON) {
                modifier = modifier * 2;
            }
            //Resist: Electric, Grass, Ground, Water
            if (move.getType() == TYPES::ELECTRIC || move.getType() == TYPES::GRASS || move.getType() == TYPES::GROUND || move.getType() == TYPES::WATER) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::GROUND:
            //Weak: WATER, GRASS, ICE
            if (move.getType() == TYPES::WATER || move.getType() == TYPES::GRASS || move.getType() == TYPES::ICE) {
                modifier = modifier * 2;
            }
            //Resist: POISON, ROCK
            if (move.getType() == TYPES::POISON || move.getType() == TYPES::ROCK) {
                modifier = modifier / 2;
            }
            //Immune: ELECTRIC
            if (move.getType() == TYPES::ELECTRIC) {
                modifier = 0;
            }
            break;

        case TYPES::ICE:
            //Weak: FIGHTING, ROCK, STEEL, FIRE
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::ROCK || move.getType() == TYPES::STEEL || move.getType() == TYPES::FIRE) {
                modifier = modifier * 2;
            }
            //Resist: ICE
            if (move.getType() == TYPES::ICE) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::NORMAL:
            //Weak: FIGHTING
            if (move.getType() == TYPES::FIGHTING) {
                modifier = modifier * 2;
            }
            //Immune: GHOST
            if (move.getType() == TYPES::GHOST) {
                modifier = 0;
            }
            break;

        case TYPES::POISON:
            //Weak: Ground, Psychic
            if (move.getType() == TYPES::GROUND || move.getType() == TYPES::PSYCHIC) {
                modifier = modifier * 2;
            }
            //Resist: Fighting, Poison, Bug, Grass, FAIRY
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::POISON || move.getType() == TYPES::BUG ||
                move.getType() == TYPES::GRASS || move.getType() == TYPES::FAIRY) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::PSYCHIC:
            //Weak: BUG, DARK, GHOST
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::POISON || move.getType() == TYPES::BUG) {
                modifier = modifier * 2;
            }
            //Resist: FIGHTING, PSYCHIC
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::PSYCHIC) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::ROCK:
            //Weak: FIGHTING, GROUND, STEEL, WATER, GRASS
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::GROUND || move.getType() == TYPES::STEEL ||
                move.getType() == TYPES::GRASS || move.getType() == TYPES::GRASS) {
                modifier = modifier * 2;
            }
            //Resist: NORMAL, FLYING, POISON, FIRE
            if (move.getType() == TYPES::NORMAL || move.getType() == TYPES::FLYING || move.getType() == TYPES::POISON || move.getType() == TYPES::FIRE) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::STEEL:
            //Weak: GROUND, FIGHTING, FIRE
            if (move.getType() == TYPES::GROUND || move.getType() == TYPES::FIGHTING || move.getType() == TYPES::FIRE) {
                modifier = modifier * 2;
            }
            //Resist: NORMAL, FLYING, ROCK, BUG, STEEL, GRASS, PSYCHIC, ICE, FAIRY, DRAGON
            if (move.getType() == TYPES::NORMAL || move.getType() == TYPES::FLYING || move.getType() == TYPES::ROCK || move.getType() == TYPES::BUG ||
                move.getType() == TYPES::STEEL || move.getType() == TYPES::GRASS || move.getType() == TYPES::PSYCHIC || move.getType() == TYPES::ICE ||
                move.getType() == TYPES::FAIRY || move.getType() == TYPES::DRAGON) {
                modifier = modifier / 2;
            }
            //Immune: POISON
            if (move.getType() == TYPES::POISON) {
                modifier = 0;
            }
            break;

        case TYPES::WATER:
            //Weak: Electric, Grass
            if (move.getType() == TYPES::ELECTRIC || move.getType() == TYPES::GRASS) {
                modifier = modifier * 2;
            }
            //Resist: Steel, Fire, Water, Ice
            if (move.getType() == TYPES::STEEL || move.getType() == TYPES::FIRE || move.getType() == TYPES::WATER || move.getType() == TYPES::ICE) {
                modifier = modifier / 2;
            }
            break;
        }
    }

    if (defender.getTypeB() != TYPES::NONE) { // Adjust as necessary to handle null or no type
        switch (defender.getTypeB()) {
        case TYPES::BUG:
            // Weak: Flying, Rock, Fire
            if (move.getType() == TYPES::FLYING || move.getType() == TYPES::ROCK || move.getType() == TYPES::FIRE) {
                modifier *= 2;
            }
            // Resist: Fighting, Ground, Grass
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::GROUND || move.getType() == TYPES::GRASS) {
                modifier /= 2;
            }
            break;

        case TYPES::DARK:
            //Weak: BUG, FAIRY, FIGHTING
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::FAIRY || move.getType() == TYPES::FIGHTING) {
                modifier = modifier * 2;
            }
            //Resist: GHOST, DARK
            if (move.getType() == TYPES::GHOST || move.getType() == TYPES::DARK) {
                modifier = modifier / 2;
            }
            //Immune: Psychic
            if (move.getType() == TYPES::PSYCHIC) {
                modifier = 0;
            }
            break;

        case TYPES::DRAGON:
            //Weak: DRAGON, FAIRY, ICE
            if (move.getType() == TYPES::DRAGON || move.getType() == TYPES::FAIRY || move.getType() == TYPES::ICE) {
                modifier = modifier * 2;
            }
            //Resist: FIRE, WATER, GRASS, ELECTRIC
            if (move.getType() == TYPES::FIRE || move.getType() == TYPES::WATER || move.getType() == TYPES::GRASS || move.getType() == TYPES::ELECTRIC) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::ELECTRIC:
            //Weak: GROUND
            if (move.getType() == TYPES::GROUND) {
                modifier = modifier * 2;
            }
            //Resist: FLYING, ELECTRIC, STEEL
            if (move.getType() == TYPES::FLYING || move.getType() == TYPES::ELECTRIC || move.getType() == TYPES::STEEL) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FAIRY:
            //Weak: POISON, STEEL
            if (move.getType() == TYPES::POISON || move.getType() == TYPES::STEEL) {
                modifier = modifier * 2;
            }
            //Resist: BUG, DARK, FIGHTING
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::DARK || move.getType() == TYPES::FIGHTING) {
                modifier = modifier / 2;
            }
            //Immune: DRAGON
            if (move.getType() == TYPES::DRAGON) {
                modifier = 0;
            }
            break;

        case TYPES::FIGHTING:
            //Weak: FAIRY, FLYING, PSYCHIC
            if (move.getType() == TYPES::FAIRY || move.getType() == TYPES::FLYING || move.getType() == TYPES::PSYCHIC) {
                modifier = modifier * 2;
            }
            //Resist: BUG, DARK, ROCK
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::DARK || move.getType() == TYPES::ROCK) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FIRE:
            //Weak: Ground, Rock, Water
            if (move.getType() == TYPES::GROUND || move.getType() == TYPES::ROCK || move.getType() == TYPES::WATER) {
                modifier = modifier * 2;
            }
            //Resist: Bug, Steel, Fire, Grass, Ice, Fairy
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::STEEL || move.getType() == TYPES::FIRE ||
                move.getType() == TYPES::GRASS || move.getType() == TYPES::ICE || move.getType() == TYPES::FAIRY) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::FLYING:
            //Weak: Electric, Ice, Rock
            if (move.getType() == TYPES::ELECTRIC || move.getType() == TYPES::ICE || move.getType() == TYPES::ROCK) {
                modifier = modifier * 2;
            }
            //Resist: Bug, Fighting, Grass
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::FIGHTING || move.getType() == TYPES::GRASS) {
                modifier = modifier / 2;
            }
            //Immune: Ground
            if (move.getType() == TYPES::GROUND) {
                modifier = 0;
            }
            break;

        case TYPES::GHOST:
            //Weak: DARK, GHOST
            if (move.getType() == TYPES::DARK || move.getType() == TYPES::GHOST) {
                modifier = modifier * 2;
            }
            //Resist: BUG, POISON
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::POISON) {
                modifier = modifier / 2;
            }
            //Immune: NORMAL, FIGHTING
            if (move.getType() == TYPES::NORMAL || move.getType() == TYPES::FIGHTING) {
                modifier = 0;
            }
            break;

        case TYPES::GRASS:
            //Weak: Bug, Fire, Flying, Ice, Poison
            if (move.getType() == TYPES::BUG || move.getType() == TYPES::FIRE || move.getType() == TYPES::FLYING ||
                move.getType() == TYPES::ICE || move.getType() == TYPES::POISON) {
                modifier = modifier * 2;
            }
            //Resist: Electric, Grass, Ground, Water
            if (move.getType() == TYPES::ELECTRIC || move.getType() == TYPES::GRASS || move.getType() == TYPES::GROUND || move.getType() == TYPES::WATER) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::GROUND:
            //Weak: WATER, GRASS, ICE
            if (move.getType() == TYPES::WATER || move.getType() == TYPES::GRASS || move.getType() == TYPES::ICE) {
                modifier = modifier * 2;
            }
            //Resist: POISON, ROCK
            if (move.getType() == TYPES::POISON || move.getType() == TYPES::ROCK) {
                modifier = modifier / 2;
            }
            //Immune: ELECTRIC
            if (move.getType() == TYPES::ELECTRIC) {
                modifier = 0;
            }
            break;

        case TYPES::ICE:
            //Weak: FIGHTING, ROCK, STEEL, FIRE
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::ROCK || move.getType() == TYPES::STEEL || move.getType() == TYPES::FIRE) {
                modifier = modifier * 2;
            }
            //Resist: ICE
            if (move.getType() == TYPES::ICE) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::NORMAL:
            //Weak: FIGHTING
            if (move.getType() == TYPES::FIGHTING) {
                modifier = modifier * 2;
            }
            //Immune: GHOST
            if (move.getType() == TYPES::GHOST) {
                modifier = 0;
            }
            break;

        case TYPES::POISON:
            //Weak: Ground, Psychic
            if (move.getType() == TYPES::GROUND || move.getType() == TYPES::PSYCHIC) {
                modifier = modifier * 2;
            }
            //Resist: Fighting, Poison, Bug, Grass, FAIRY
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::POISON || move.getType() == TYPES::BUG ||
                move.getType() == TYPES::GRASS || move.getType() == TYPES::FAIRY) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::PSYCHIC:
            //Weak: BUG, DARK, GHOST
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::POISON || move.getType() == TYPES::BUG) {
                modifier = modifier * 2;
            }
            //Resist: FIGHTING, PSYCHIC
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::PSYCHIC) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::ROCK:
            //Weak: FIGHTING, GROUND, STEEL, WATER, GRASS
            if (move.getType() == TYPES::FIGHTING || move.getType() == TYPES::GROUND || move.getType() == TYPES::STEEL ||
                move.getType() == TYPES::GRASS || move.getType() == TYPES::GRASS) {
                modifier = modifier * 2;
            }
            //Resist: NORMAL, FLYING, POISON, FIRE
            if (move.getType() == TYPES::NORMAL || move.getType() == TYPES::FLYING || move.getType() == TYPES::POISON || move.getType() == TYPES::FIRE) {
                modifier = modifier / 2;
            }
            break;

        case TYPES::STEEL:
            //Weak: GROUND, FIGHTING, FIRE
            if (move.getType() == TYPES::GROUND || move.getType() == TYPES::FIGHTING || move.getType() == TYPES::FIRE) {
                modifier = modifier * 2;
            }
            //Resist: NORMAL, FLYING, ROCK, BUG, STEEL, GRASS, PSYCHIC, ICE, FAIRY, DRAGON
            if (move.getType() == TYPES::NORMAL || move.getType() == TYPES::FLYING || move.getType() == TYPES::ROCK || move.getType() == TYPES::BUG ||
                move.getType() == TYPES::STEEL || move.getType() == TYPES::GRASS || move.getType() == TYPES::PSYCHIC || move.getType() == TYPES::ICE ||
                move.getType() == TYPES::FAIRY || move.getType() == TYPES::DRAGON) {
                modifier = modifier / 2;
            }
            //Immune: POISON
            if (move.getType() == TYPES::POISON) {
                modifier = 0;
            }
            break;

        case TYPES::WATER:
            //Weak: Electric, Grass
            if (move.getType() == TYPES::ELECTRIC || move.getType() == TYPES::GRASS) {
                modifier = modifier * 2;
            }
            //Resist: Steel, Fire, Water, Ice
            if (move.getType() == TYPES::STEEL || move.getType() == TYPES::FIRE || move.getType() == TYPES::WATER || move.getType() == TYPES::ICE) {
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
    return dmgArray;
}

static int targetHit(Pokemon& attacker, Pokemon& defender, Move move) {
    int accMove = move.getAcc();

    int stage = static_cast<int>(attacker.getAccStage() - defender.getEvaStage());
    if (stage > 6) stage = 6;
    else if (stage < -6) stage = -6;
    double stageMultiplier = accMultiplier.at(stage);

    int r = std::rand() % 100 + 1;

    if (r <= (accMove * stageMultiplier)) {
        return 1; // Hit
    }
    else {
        return 0; // Miss
    }
}

static void damageCalc(Pokemon& attacker, Pokemon& defender, Move move) {
    // Initialize random number generator
    std::srand(static_cast<unsigned int>(std::time(0)));

    double critCalc = 0;
    double stab = 1.0;
    double randFact = 0;
    double force = 0;
    double object = 0;

    // Determine if the move hits its target
    if (!targetHit(attacker, defender, move)) {
        std::cout << "The attack missed!" << std::endl;
    } else {

        // Physical or special attack calculation
        if (move.getCat() == 1) {
            force = attacker.getAtk();
            object = defender.getDef();
        }
        else if (move.getCat() == 2) {
            force = attacker.getSpa();
            object = defender.getSpd();
        }

        // Critical hit calculation
        // TODO
        critCalc = std::rand() % 256;
        if (critCalc < (attacker.getSpe() / 2)) {
            critCalc = 1.5;
            std::cout << "It's a critical hit!" << std::endl;
        }
        else {
            critCalc = 1.0;
        }

        // Random factor calculation
        randFact = (std::rand() % 16 + 85) / 100.0;

        // Same Type Attack Bonus (STAB)
        if (move.getType() == attacker.getTypeA() || move.getType() == attacker.getTypeB()) {
            stab = 1.5;
        }

        // Base damage calculation
        int baseDamage = static_cast<int>((((22) * move.getPow() * (force / object)) / (50.0 + 2)) * critCalc * randFact * stab);

        // Get type multiplier
        std::vector<int> dmgArray = typeMultiplier(baseDamage, move, defender);
        baseDamage = dmgArray[0];
        if (baseDamage < 1) baseDamage = 1;

        // Determine effectiveness
        if (dmgArray[1] == 1) {
            std::cout << "It's super effective!" << std::endl;
        }
        else if (dmgArray[2] == 1) {
            std::cout << "It's not very effective..." << std::endl;
        }
        else if (dmgArray[3] == 1) {
            std::cout << "It doesn't affect " << defender.getPokeName() << "..." << std::endl;
        }

        // Apply damage to defender's HP
        defender.setHP(defender.getHP() - baseDamage);
        std::cout << baseDamage << " damage!" << std::endl;
    }
}

void statusCalc(Pokemon& attacker, Pokemon& defender, Move move) {
    // {Self/enemy, atk, def, spa, spd, spe, acc, eva}
    int* effect = move.statEffect;
    double mult;
    int multStage;

    if (effect[0] == 0) { // If changing self
        if (effect[1] > 0) { // Raise attack
            multStage = effect[1];
            attacker.setAtkStage(multStage);
            multStage = attacker.getAtkStage();
            mult = statMultiplier.at(multStage);
            int newAtk = static_cast<int>(mult * attacker.getAtk());
            attacker.setAtk(newAtk);
            std::cout << attacker.getPokeName() << "'s attack rose!" << std::endl;
        }
        if (effect[1] < 0) { // Drop attack
            multStage = effect[1];
            attacker.setAtkStage(multStage);
            multStage = attacker.getAtkStage();
            mult = statMultiplier.at(multStage);
            int newAtk = static_cast<int>(mult * attacker.getAtk());
            attacker.setAtk(newAtk);
            std::cout << attacker.getPokeName() << "'s attack fell!" << std::endl;
        }
        if (effect[2] > 0) { // Raise defense
            multStage = effect[2];
            attacker.setDefStage(multStage);
            multStage = attacker.getDefStage();
            mult = statMultiplier.at(multStage);
            int newDef = static_cast<int>(mult * attacker.getDef());
            attacker.setDef(newDef);
            std::cout << attacker.getPokeName() << "'s defense rose!" << std::endl;
        }
        if (effect[2] < 0) { // Drop defense
            multStage = effect[2];
            attacker.setDefStage(multStage);
            multStage = attacker.getDefStage();
            mult = statMultiplier.at(multStage);
            int newDef = static_cast<int>(mult * attacker.getDef());
            attacker.setDef(newDef);
            std::cout << attacker.getPokeName() << "'s defense fell!" << std::endl;
        }
        if (effect[3] > 0) { // Raise special attack
            multStage = effect[3];
            attacker.setSpaStage(multStage);
            multStage = attacker.getSpaStage();
            mult = statMultiplier.at(multStage);
            int newSpa = static_cast<int>(mult * attacker.getSpa());
            attacker.setSpa(newSpa);
            std::cout << attacker.getPokeName() << "'s special attack rose!" << std::endl;
        }
        if (effect[3] < 0) { // Drop special attack
            multStage = effect[3];
            attacker.setSpaStage(multStage);
            multStage = attacker.getSpaStage();
            mult = statMultiplier.at(multStage);
            int newSpa = static_cast<int>(mult * attacker.getSpa());
            attacker.setSpa(newSpa);
            std::cout << attacker.getPokeName() << "'s special attack fell!" << std::endl;
        }
        if (effect[4] > 0) { // Raise special defense
            multStage = effect[4];
            attacker.setSpdStage(multStage);
            multStage = attacker.getSpdStage();
            mult = statMultiplier.at(multStage);
            int newSpd = static_cast<int>(mult * attacker.getSpd());
            attacker.setSpd(newSpd);
            std::cout << attacker.getPokeName() << "'s special defense rose!" << std::endl;
        }
        if (effect[4] > 0) { // Drop special defense
            multStage = effect[4];
            attacker.setSpdStage(multStage);
            multStage = attacker.getSpdStage();
            mult = statMultiplier.at(multStage);
            int newSpd = static_cast<int>(mult * attacker.getSpd());
            attacker.setSpd(newSpd);
            std::cout << attacker.getPokeName() << "'s special defense fell!" << std::endl;
        }
        if (effect[5] > 0) { // Raise speed
            multStage = effect[5];
            attacker.setSpeStage(multStage);
            multStage = attacker.getSpeStage();
            mult = statMultiplier.at(multStage);
            int newSpe = static_cast<int>(mult * attacker.getSpe());
            attacker.setSpe(newSpe);
            std::cout << attacker.getPokeName() << "'s speed rose!" << std::endl;
        }
        if (effect[5] < 0) { // Drop speed
            multStage = effect[5];
            attacker.setSpeStage(multStage);
            multStage = attacker.getSpeStage();
            mult = statMultiplier.at(multStage);
            int newSpe = static_cast<int>(mult * attacker.getSpe());
            attacker.setSpe(newSpe);
            std::cout << attacker.getPokeName() << "'s speed fell!" << std::endl;
        }
        if (effect[6] > 0) { // Buff accuracy
			multStage = effect[6];
			attacker.setAccStage(multStage);
            std::cout << attacker.getPokeName() << "'s accuracy rose!" << std::endl;
        }
        if (effect[7] > 0) { // Buff evasion
			multStage = effect[7];
			attacker.setEvaStage(multStage);
            std::cout << attacker.getPokeName() << "'s evasion rose!" << std::endl;
        }
    }
    else if (effect[0] == 1) { // Changing opponent
        if (effect[1] < 0) { // Drop attack
            multStage = effect[1];
            defender.setAtkStage(multStage);
            multStage = defender.getAtkStage();
            mult = statMultiplier.at(multStage);
            int newAtk = static_cast<int>(mult * defender.getAtk());
            defender.setAtk(newAtk);
            std::cout << defender.getPokeName() << "'s attack fell!" << std::endl;
        }
        if (effect[2] < 0) { // Drop defense
            multStage = effect[2];
            defender.setDefStage(multStage);
            multStage = defender.getDefStage();
            mult = statMultiplier.at(multStage);
            int newDef = static_cast<int>(mult * defender.getDef());
            defender.setDef(newDef);
            std::cout << defender.getPokeName() << "'s defense fell!" << std::endl;
        }
        if (effect[3] < 0) { // Drop special attack
            multStage = effect[3];
            defender.setSpaStage(multStage);
            multStage = defender.getSpaStage();
            mult = statMultiplier.at(multStage);
            int newSpa = static_cast<int>(mult * defender.getSpa());
            defender.setSpa(newSpa);
            std::cout << defender.getPokeName() << "'s special attack fell!" << std::endl;
        }
        if (effect[4] < 0) { // Drop special defense
            multStage = effect[4];
            defender.setSpdStage(multStage);
            multStage = defender.getSpdStage();
            mult = statMultiplier.at(multStage);
            int newSpd = static_cast<int>(mult * defender.getSpd());
            defender.setSpd(newSpd);
            std::cout << defender.getPokeName() << "'s special defense fell!" << std::endl;
        }
        if (effect[5] < 0) { // Drop speed
            multStage = effect[5];
            defender.setSpeStage(multStage);
            multStage = defender.getSpeStage();
            mult = statMultiplier.at(multStage);
            int newSpe = static_cast<int>(mult * defender.getSpe());
            defender.setSpe(newSpe);
            std::cout << defender.getPokeName() << "'s speed fell!" << std::endl;
        }
        if (effect[6] < 0) { // Drop accuracy
			multStage = effect[6];
			defender.setAccStage(multStage);
            std::cout << defender.getPokeName() << "'s accuracy fell!" << std::endl;
        }
        if (effect[7] < 0) { // Drop evasion
			multStage = effect[7];
			defender.setEvaStage(multStage);
            std::cout << defender.getPokeName() << "'s evasion fell!" << std::endl;
        }
    }
}

/**
 * SUBJECT TO CHANGE
 * Computes the CPU Pokemon battle algorithm
 * poketeam1 - Input array of poketeam1 stats
 * poketeam2 - Input array of poketeam2 stats
 * winner - Output array of winning pokemon
 */
void pokeBattleCPU(Pokemon poketeam1[], Pokemon poketeam2[], Pokemon winner[], int teamsize) {
    // Integrate process from Slowdown
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> dist(0, 100);

    for (int i = 0; i < teamsize; i++) {
        // Until KO
        while (poketeam1[i].getHP() >= 1 && poketeam2[i].getHP() >= 1) {
            Move selected;
            int canUse = 0;
            if (poketeam1[i].getSpe() > poketeam2[i].getSpe()) {
                // Friend attacks first
                while (!canUse) {   // Check if move can be used
                    selected = poketeam1[i].getMove(rng() % poketeam1[i].getMovesNum());
                    if (selected.getPP() < 1) {
                        std::cout << selected.getName() << " is out of PP!" << std::endl;
                    }
                    else {
                        selected.useMove();
                        canUse++;
                    }
                }
                canUse--;

                std::cout << poketeam1[i].getPokeName() << " used " << selected.getName() << std::endl;

                if (selected.getCat() == 3) {   // Check if move is a status move or an attack
                    statusCalc(poketeam1[i], poketeam2[i], selected);
                }
                else {
                    damageCalc(poketeam1[i], poketeam2[i], selected);
                }
                std::cout << std::endl;

                if (poketeam2[i].getHP() < 1) break;


                // Foe attacks
                while (!canUse) {
                    selected = poketeam2[i].getMove(rng() % poketeam2[i].getMovesNum());
                    if (selected.getPP() < 1) {
                        std::cout << selected.getName() << " is out of PP!" << std::endl;
                    }
                    else {
                        selected.useMove();
                        canUse++;
                    }
                }
                canUse--;

                std::cout << poketeam2[i].getPokeName() << " used " << selected.getName() << std::endl;

                if (selected.getCat() == 3) {
                    statusCalc(poketeam2[i], poketeam1[i], selected);
                }
                else {
                    damageCalc(poketeam2[i], poketeam1[i], selected);
                }
                std::cout << std::endl;

            }
            else {
                // Foe attacks first
                while (!canUse) {
                    selected = poketeam2[i].getMove(rng() % poketeam2[i].getMovesNum());
                    if (selected.getPP() < 1) {
                        std::cout << selected.getName() << " is out of PP!" << std::endl;
                    }
                    else {
                        selected.useMove();
                        canUse++;
                    }
                }
                canUse--;

                std::cout << poketeam2[i].getPokeName() << " used " << selected.getName() << std::endl;

                if (selected.getCat() == 3) {
                    statusCalc(poketeam2[i], poketeam1[i], selected);
                }
                else {
                    damageCalc(poketeam2[i], poketeam1[i], selected);
                }
                std::cout << std::endl;

                if (poketeam1[i].getHP() < 1) break;

                // Friend attacks
                while (!canUse) {   // Check if move can be used
                    selected = poketeam1[i].getMove(rng() % poketeam1[i].getMovesNum());
                    if (selected.getPP() < 1) {
                        std::cout << selected.getName() << " is out of PP!" << std::endl;
                    }
                    else {
                        selected.useMove();
                        canUse++;
                    }
                }
                canUse--;

                std::cout << poketeam1[i].getPokeName() << " used " << selected.getName() << std::endl;

                if (selected.getCat() == 3) {
                    statusCalc(poketeam1[i], poketeam2[i], selected);
                }
                else {
                    damageCalc(poketeam1[i], poketeam2[i], selected);
                }
                std::cout << std::endl;
            }
        }

        if (poketeam2[i].getHP() < 1) {
            winner[i] = poketeam1[i];
        }
        else {
            winner[i] = poketeam2[i];
        }
    }

} // pokeBattleCPU