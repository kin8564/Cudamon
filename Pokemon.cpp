#include <string>
#include <array>
#include <cstring>
#include "pokemon.h"
#include "Types.h"
#include "Move.h"
#include "Natures.h"

char Pokename[30];

int healthPoints, attack, defense, specialAttack, specialDefense, speed, pokeacc, evasion;

TYPES type1;
TYPES type2;

NATURES weirdo;

Move moves[4];

int statAtk[2];  // {stage, actual}
int statDef[2];
int statSpa[2];
int statSpd[2];
int statSpe[2];

double statAcc[2];  // {stage, multiplier}
double statEva[2];

Pokemon::Pokemon(const char* monster, int hp, int atk, int def, int spa, int spd, int spe, TYPES typeA, TYPES typeB)
: healthPoints(hp), attack(atk), defense(def), specialAttack(spa), specialDefense(spd), speed(spe),
type1(typeA), type2(typeB), pokeacc(0), evasion(0),statAtk{0, attack}, statDef{0, defense}, statSpa{0, specialAttack}, statSpd{0, specialDefense}, statSpe{0, speed},
statAcc{0, 1.0}, statEva{0, 1.0}{
    strncpy_s(Pokename, monster, sizeof(Pokename));
    Pokename[sizeof(Pokename) - 1] = '\0'; // Null-terminate just in case

    for (int i = 0; i < 4; ++i) {
        moves[i] = Move();
    }
}

Pokemon::Pokemon(){}

//Change the base stats of the Pokemon based on the nature
void Pokemon::setNature(NATURES nature) {
   switch (nature) {
       case NATURES::HARDY:    //Inc Atk, Dec Atk
           break;
       case NATURES::LONELY:   //Inc Atk, Dec Def
           this->attack = this->attack * 1.1;
           setAtk(this->attack);
           this->defense = this->defense * 0.9;
           setDef(this->defense);
           break;
       case NATURES::BRAVE:    //Inc Atk, Dec Spe
           this->attack = this->attack * 1.1;
           setAtk(this->attack);
           this->speed = this->speed * 0.9;
           setSpe(this->speed);
           break;
       case NATURES::ADAMANT:  //Inc Atk, Dec Spa
           this->attack = this->attack * 1.1;
           setAtk(this->attack);
           this->specialAttack = this->specialAttack * 0.9;
           setSpa(this->specialAttack);
           break;
       case NATURES::NAUGHTY:  //Inc Atk, Dec SpD
           this->attack = this->attack * 1.1;
           setAtk(this->attack);
           this->specialDefense = this->specialDefense * 0.9;
           setSpd(this->specialDefense);
           break;
       case NATURES::BOLD:     //Inc Def, Dec Atk
           this->defense = this->defense * 1.1;
           setDef(this->defense);
           this->attack = this->attack * 0.9;
           setAtk(this->attack);
           break;
       case NATURES::DOCILE:   //Inc Def, Dec Def
           break;
       case NATURES::RELAXED:  //Inc Def, Dec Spe
           this->defense = this->defense * 1.1;
           setDef(this->defense);
           this->speed = this->speed * 0.9;
           setSpe(this->speed);
           break;
       case NATURES::IMPISH:   //Inc Def, Dec Spa
           this->defense = this->defense * 1.1;
           setDef(this->defense);
           this->specialAttack = this->specialAttack * 0.9;
           setSpa(this->specialAttack);
           break;
       case NATURES::LAX:      //Inc Def, Dec SpD
           this->defense = this->defense * 1.1;
           setDef(this->defense);
           this->specialDefense = this->specialDefense * 0.9;
           setSpd(this->specialDefense);
           break;
       case NATURES::TIMID:    //Inc Spe, Dec Atk
           this->speed = this->speed * 1.1;
           setSpe(this->speed);
           this->attack = this->attack * 0.9;
           setAtk(this->attack);
           break;
       case NATURES::HASTY:    //Inc Spe, Dec Def
           this->speed = this->speed * 1.1;
           setSpe(this->speed);
           this->defense = this->defense * 0.9;
           setDef(this->defense);
           break;
       case NATURES::SERIOUS:  //Inc Spe, Dec Spe
           break;
       case NATURES::JOLLY:    //Inc Spe, Dec Spa
           this->speed = this->speed * 1.1;
           setSpe(this->speed);
           this->specialAttack = this->specialAttack * 0.9;
           setSpa(this->specialAttack);
           break;
       case NATURES::NAIVE:    //Inc Spe, Dec SpD
           this->speed = this->speed * 1.1;
           setSpe(this->speed);
           this->specialDefense = this->specialDefense * 0.9;
           setSpd(this->specialDefense);
           break;
       case NATURES::MODEST:   //Inc Spa, Dec Atk
           this->specialAttack = this->specialAttack * 1.1;
           setSpa(this->specialAttack);
           this->attack = this->attack * 0.9;
           setAtk(this->attack);
           break;
       case NATURES::MILD:     //Inc Spa, Dec Def
           this->specialAttack = this->specialAttack * 1.1;
           setSpa(this->specialAttack);
           this->defense = this->defense * 0.9;
           setDef(this->defense);
           break;
       case NATURES::QUIET:    //Inc Spa, Dec Spe
           this->specialAttack = this->specialAttack * 1.1;
           setSpa(this->specialAttack);
           this->speed = this->speed * 0.9;
           setSpe(this->speed);
           break;
       case NATURES::BASHFUL:  //Inc Spa, Dec SpD
           break;
       case NATURES::RASH:     //Inc Spa, Dec SpD
           this->specialAttack = this->specialAttack * 1.1;
           setSpa(this->specialAttack);
           this->specialDefense = this->specialDefense * 0.9;
           setSpd(this->specialDefense);
           break;
       case NATURES::CALM:     //Inc SpD, Dec Atk
           this->specialDefense = this->specialDefense * 1.1;
           setSpd(this->specialDefense);
           this->attack = this->attack * 0.9;
           setAtk(this->attack);
           break;
       case NATURES::GENTLE:   //Inc SpD, Dec Def
           this->specialDefense = this->specialDefense * 1.1;
           setSpd(this->specialDefense);
           this->defense = this->defense * 0.9;
           setDef(this->defense);
           break;
       case NATURES::SASSY:    //Inc SpD, Dec Spe
           this->specialDefense = this->specialDefense * 1.1;
           setSpd(this->specialDefense);
           this->speed = this->speed * 0.9;
           setSpe(this->speed);
           break;
       case NATURES::CAREFUL:  //Inc SpD, Dec Spa
           this->specialDefense = this->specialDefense * 1.1;
           setSpd(this->specialDefense);
           this->specialAttack = this->specialAttack * 0.9;
           setSpa(this->specialAttack);
           break;
       case NATURES::QUIRKY:   //Inc SpD, Dec SpD
           break;
   }
}

char* Pokemon::getPokeName() {
    return Pokename;
}

int Pokemon::getHP() {
    return healthPoints;
}

void Pokemon::setHP(int x) {
    healthPoints = x;
}

int Pokemon::getAtk() {
    return statAtk[1];
}

int Pokemon::getAtkStage() {
    return statAtk[0];
}

void Pokemon::setAtk(int x) {
    statAtk[1] = x;
}

void Pokemon::setAtkStage(int x) {
    statAtk[0] += x;
    if (statAtk[0] > 6) {
        statAtk[0] = 6;
    } else if (statAtk[0] < -6) {
        statAtk[0] = -6;
    }
}

int Pokemon::getDef() {
    return statDef[1];
}

int Pokemon::getDefStage() {
    return statDef[0];
}

void Pokemon::setDef(int x) {
    statDef[1] = x;
}

void Pokemon::setDefStage(int x) {
    statDef[0] += x;
    if (statDef[0] > 6) {
        statDef[0] = 6;
    } else if (statDef[0] < -6) {
        statDef[0] = -6;
    }
}

int Pokemon::getSpa() {
    return statSpa[1];
}

int Pokemon::getSpaStage() {
    return statSpa[0];
}

void Pokemon::setSpa(int x) {
    statSpa[1] = x;
}

void Pokemon::setSpaStage(int x) {
    statSpa[0] += x;
    if (statSpa[0] > 6) {
        statSpa[0] = 6;
    } else if (statSpa[0] < -6) {
        statSpa[0] = -6;
    }
}

int Pokemon::getSpd() {
    return statSpd[1];
}

int Pokemon::getSpdStage() {
    return statSpd[0];
}

void Pokemon::setSpd(int x) {
    statSpd[1] = x;
}

void Pokemon::setSpdStage(int x) {
    statSpd[0] += x;
    if (statSpd[0] > 6) {
        statSpd[0] = 6;
    } else if (statSpd[0] < -6) {
        statSpd[0] = -6;
    }
}

int Pokemon::getSpe() {
    return statSpe[1];
}

int Pokemon::getSpeStage() {
    return statSpe[0];
}

void Pokemon::setSpe(int x) {
    statSpe[1] = x;
}

void Pokemon::setSpeStage(int x) {
    statSpe[0] += x;
    if (statSpe[0] > 6) {
        statSpe[0] = 6;
    } else if (statSpe[0] < -6) {
        statSpe[0] = -6;
    }
}

TYPES Pokemon::getTypeA(){
    return type1;
}

TYPES Pokemon::getTypeB(){
    return type2;
}

Move Pokemon::getMove1(){
    return moves[0];
}

Move Pokemon::getMove2(){
    return moves[1];
}

Move Pokemon::getMove3(){
    return moves[2];
}

Move Pokemon::getMove4(){
    return moves[3];
}

Move Pokemon::getMove(int x){
    return moves[x];
}

int Pokemon::getMovesNum(){
    return sizeof(moves) / sizeof(moves[0]);
}

void Pokemon::setMove1(Move move){
    moves[0] = move;
}

void Pokemon::setMove2(Move move){
    moves[1] = move;
}

void Pokemon::setMove3(Move move){
    moves[2] = move;
}

void Pokemon::setMove4(Move move){
    moves[3] = move;
}

int Pokemon::getPokeAcc(){
    return pokeacc;
}

double Pokemon::getPokeAccStage(){
    return statAcc[0];
}

void Pokemon::setAcc(int x) {
    pokeacc = x;
}

void Pokemon::incAcc(int x){
    if (pokeacc < 6){
        pokeacc += x;
    }
}

void Pokemon::decAcc(int x){
    if (pokeacc > -6){
        pokeacc -= x;
    }
}

int Pokemon::getEva() {
    return evasion;
}

void Pokemon::setEva(int x) {
    evasion = x;
}

void Pokemon::incEva(int x) {
    if (evasion < 6) {
        evasion += x;
    }
}

void Pokemon::decEva(int x) {
    if (evasion > 6) {
        evasion -= x;
    }
}
