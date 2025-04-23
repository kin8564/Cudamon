#ifndef POKEMON_H
#define POKEMON_H
#include "Types.h"
#include "Move.h"

#include <string>
#include <array>


/**
 * Parent class for all Pokemon
 */
struct Pokemon {
    char Pokename[30];

    int healthPoints, attack, defense, specialAttack, specialDefense, speed, pokeacc, evasion;

    TYPES type1;
    TYPES type2;

    Move moves[4];

    int statAtk[2];  // {stage, actual}
    int statDef[2];
    int statSpa[2];
    int statSpd[2];
    int statSpe[2];

    double statAcc[2];  // {stage, multiplier}
    double statEva[2];

public:
    Pokemon();
    Pokemon(const char* Pokename, int hp, int atk, int def, int spa, int spd, int spe, TYPES typeA, TYPES typeB);
    char* getPokeName();
    int getHP();
    int getAtk();
    int getAtkStage();
    int getDef();
    int getDefStage();
    int getSpa();
    int getSpaStage();
    int getSpd();
    int getSpdStage();
    int getSpe();
    int getSpeStage();
    TYPES getTypeA();
    TYPES getTypeB();
    Move getMove1();
    Move getMove2();
    Move getMove3();
    Move getMove4();
    Move getMove(int x);
    int getMovesNum();
    int getPokeAcc();
    double getPokeAccStage();
    int getEva();


    void setHP(int x);
    void setAtk(int x);
    void setAtkStage(int x);
    void setDef(int x);
    void setDefStage(int x);
    void setSpa(int x);
    void setSpaStage(int x);
    void setSpd(int x);
    void setSpdStage(int x);
    void setSpe(int x);
    void setSpeStage(int x);
    void setMove1(Move move);
    void setMove2(Move move);
    void setMove3(Move move);
    void setMove4(Move move);
    void setAcc(int x);
    void incAcc(int x);
    void decAcc(int x);
    void setEva(int x);
    void incEva(int x);
    void decEva(int x);
};

#endif