#include <string>
#include <array>
#include "pokemon.h"
#include "Types.h"
#include "Move.h"

std::string Pokename;

int healthPoints, attack, defense, specialAttack, specialDefense, speed, pokeacc, evasion;

TYPES type1;
TYPES type2;

std::array<Move*, 4> moves;

std::array<int, 2> statAtk, statDef, statSpa, statSpd, statSpe;

std::array<double, 2> statAcc, statEva;

Pokemon::Pokemon(std::string monster, int hp, int atk, int def, int spa, int spd, int spe, TYPES typeA, TYPES typeB)
: Pokename(monster), healthPoints(hp), attack(atk), defense(def), specialAttack(spa), specialDefense(spd), speed(spe),
type1(typeA), type2(typeB), pokeacc(0), evasion(0),
statAtk{0, attack}, statDef{0, defense}, statSpa{0, specialAttack}, statSpd{0, specialDefense}, statSpe{0, speed},
statAcc{0, 1.0}, statEva{0, 1.0} {moves.fill(nullptr);}

std::string Pokemon::getPokeName() {
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

Move* Pokemon::getMove1(){
    return moves[0];
}

Move* Pokemon::getMove2(){
    return moves[1];
}

Move* Pokemon::getMove3(){
    return moves[2];
}

Move* Pokemon::getMove4(){
    return moves[3];
}

Move* Pokemon::getMove(int x){
    return moves[x];
}

int Pokemon::getMovesNum(){
    return moves.size();
}

void Pokemon::setMove1(Move* move){
    moves[0] = move;
}

void Pokemon::setMove2(Move* move){
    moves[1] = move;
}

void Pokemon::setMove3(Move* move){
    moves[2] = move;
}

void Pokemon::setMove4(Move* move){
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
