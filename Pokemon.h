#ifndef POKEMON_H
#define POKEMON_H
#include "Types.h"
#include "Move.h"

#include <string>
#include <array>


/**
 * Parent class for all Pokemon
 */
class Pokemon {
private:
    std::string Pokename;

    int healthPoints, attack, defense, specialAttack, specialDefense, speed, pokeacc, evasion;

    TYPES type1;
    TYPES type2;

    std::array<Move*, 4> moves;

    std::array<int, 2> statAtk, statDef, statSpa, statSpd, statSpe;

    std::array<double, 2> statAcc, statEva;

public:
    Pokemon(std::string monster, int hp, int atk, int def, int spa, int spd, int spe, TYPES typeA, TYPES typeB);
    std::string getPokeName();
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
    Move* getMove1();
    Move* getMove2();
    Move* getMove3();
    Move* getMove4();
    Move* getMove(int x);
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
    void setMove1(Move* move);
    void setMove2(Move* move);
    void setMove3(Move* move);
    void setMove4(Move* move);
    void setAcc(int x);
    void incAcc(int x);
    void decAcc(int x);
    void setEva(int x);
    void incEva(int x);
    void decEva(int x);
};

#endif