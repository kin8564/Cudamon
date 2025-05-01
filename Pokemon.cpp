#include <string>
#include <array>
#include <cstring>
#include "pokemon.h"
#include "Types.h"
#include "Move.h"
#include "Natures.h"

Pokemon::Pokemon(const char* monster, int hp, int atk, int def, int spa, int spd, int spe, TYPES typeA, TYPES typeB)
: healthPoints(hp), attack(atk), defense(def), specialAttack(spa), specialDefense(spd), speed(spe),
type1(typeA), type2(typeB), pokeacc(0), evasion(0),statAtk{0, attack}, statDef{0, defense}, statSpa{0, specialAttack}, statSpd{0, specialDefense}, statSpe{0, speed},
statAcc{0, 1.0}, statEva{0, 1.0}{
    strncpy_s(this->Pokename, sizeof(this->Pokename), monster, 30);
    this->Pokename[sizeof(this->Pokename) - 1] = '\0'; // Null-terminate just in case

    for (int i = 0; i < 4; ++i) {
        this->moves[i] = Move();
    }
}

Pokemon::Pokemon(){}

//Change the base stats of the Pokemon based on the nature
void Pokemon::setNature(NATURES nature) {
    switch (nature) {
    case NATURES::HARDY:    // Inc Atk, Dec Atk
    case NATURES::DOCILE:   // Inc Def, Dec Def
    case NATURES::SERIOUS:  // Inc Spe, Dec Spe
    case NATURES::BASHFUL:  // Inc Spa, Dec SpD
    case NATURES::QUIRKY:   // Inc SpD, Dec SpD
        break;
    case NATURES::LONELY:   // Inc Atk, Dec Def
        attack = static_cast<int>(std::round(attack * 1.1));
        setAtk(attack);
        defense = static_cast<int>(std::round(defense * 0.9));
        setDef(defense);
        break;
    case NATURES::BRAVE:    // Inc Atk, Dec Spe
        attack = static_cast<int>(std::round(attack * 1.1));
        setAtk(attack);
        speed = static_cast<int>(std::round(speed * 0.9));
        setSpe(speed);
        break;
    case NATURES::ADAMANT:  // Inc Atk, Dec Spa
        attack = static_cast<int>(std::round(attack * 1.1));
        setAtk(attack);
        specialAttack = static_cast<int>(std::round(specialAttack * 0.9));
        setSpa(specialAttack);
        break;
    case NATURES::NAUGHTY:  // Inc Atk, Dec SpD
        attack = static_cast<int>(std::round(attack * 1.1));
        setAtk(attack);
        specialDefense = static_cast<int>(std::round(specialDefense * 0.9));
        setSpd(specialDefense);
        break;
    case NATURES::BOLD:     // Inc Def, Dec Atk
        defense = static_cast<int>(std::round(defense * 1.1));
        setDef(defense);
        attack = static_cast<int>(std::round(attack * 0.9));
        setAtk(attack);
        break;
    case NATURES::RELAXED:  // Inc Def, Dec Spe
        defense = static_cast<int>(std::round(defense * 1.1));
        setDef(defense);
        speed = static_cast<int>(std::round(speed * 0.9));
        setSpe(speed);
        break;
    case NATURES::IMPISH:   // Inc Def, Dec Spa
        defense = static_cast<int>(std::round(defense * 1.1));
        setDef(defense);
        specialAttack = static_cast<int>(std::round(specialAttack * 0.9));
        setSpa(specialAttack);
        break;
    case NATURES::LAX:      // Inc Def, Dec SpD
        defense = static_cast<int>(std::round(defense * 1.1));
        setDef(defense);
        specialDefense = static_cast<int>(std::round(specialDefense * 0.9));
        setSpd(specialDefense);
        break;
    case NATURES::TIMID:    // Inc Spe, Dec Atk
        speed = static_cast<int>(std::round(speed * 1.1));
        setSpe(speed);
        attack = static_cast<int>(std::round(attack * 0.9));
        setAtk(attack);
        break;
    case NATURES::HASTY:    // Inc Spe, Dec Def
        speed = static_cast<int>(std::round(speed * 1.1));
        setSpe(speed);
        defense = static_cast<int>(std::round(defense * 0.9));
        setDef(defense);
        break;
    case NATURES::JOLLY:    // Inc Spe, Dec Spa
        speed = static_cast<int>(std::round(speed * 1.1));
        setSpe(speed);
        specialAttack = static_cast<int>(std::round(specialAttack * 0.9));
        setSpa(specialAttack);
        break;
    case NATURES::NAIVE:    // Inc Spe, Dec SpD
        speed = static_cast<int>(std::round(speed * 1.1));
        setSpe(speed);
        specialDefense = static_cast<int>(std::round(specialDefense * 0.9));
        setSpd(specialDefense);
        break;
    case NATURES::MODEST:   // Inc Spa, Dec Atk
        specialAttack = static_cast<int>(std::round(specialAttack * 1.1));
        setSpa(specialAttack);
        attack = static_cast<int>(std::round(attack * 0.9));
        setAtk(attack);
        break;
    case NATURES::MILD:     // Inc Spa, Dec Def
        specialAttack = static_cast<int>(std::round(specialAttack * 1.1));
        setSpa(specialAttack);
        defense = static_cast<int>(std::round(defense * 0.9));
        setDef(defense);
        break;
    case NATURES::QUIET:    // Inc Spa, Dec Spe
        specialAttack = static_cast<int>(std::round(specialAttack * 1.1));
        setSpa(specialAttack);
        speed = static_cast<int>(std::round(speed * 0.9));
        setSpe(speed);
        break;
    case NATURES::RASH:     // Inc Spa, Dec SpD
        specialAttack = static_cast<int>(std::round(specialAttack * 1.1));
        setSpa(specialAttack);
        specialDefense = static_cast<int>(std::round(specialDefense * 0.9));
        setSpd(specialDefense);
        break;
    case NATURES::CALM:     // Inc SpD, Dec Atk
        specialDefense = static_cast<int>(std::round(specialDefense * 1.1));
        setSpd(specialDefense);
        attack = static_cast<int>(std::round(attack * 0.9));
        setAtk(attack);
        break;
    case NATURES::GENTLE:   // Inc SpD, Dec Def
        specialDefense = static_cast<int>(std::round(specialDefense * 1.1));
        setSpd(specialDefense);
        defense = static_cast<int>(std::round(defense * 0.9));
        setDef(defense);
        break;
    case NATURES::SASSY:    // Inc SpD, Dec Spe
        specialDefense = static_cast<int>(std::round(specialDefense * 1.1));
        setSpd(specialDefense);
        speed = static_cast<int>(std::round(speed * 0.9));
        setSpe(speed);
        break;
    case NATURES::CAREFUL:  // Inc SpD, Dec Spa
        specialDefense = static_cast<int>(std::round(specialDefense * 1.1));
        setSpd(specialDefense);
        specialAttack = static_cast<int>(std::round(specialAttack * 0.9));
        setSpa(specialAttack);
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

//int Pokemon::getAcc(){
//    return pokeacc;
//}

double Pokemon::getAccStage(){
    return statAcc[0];
}

//void Pokemon::setAcc(int x) {
//    pokeacc = x;
//}

void Pokemon::setAccStage(int x) {
	statAcc[0] += x;
	if (statAcc[0] > 6) {
		statAcc[0] = 6;
	}
	else if (statAcc[0] < -6) {
		statAcc[0] = -6;
	}
}

//void Pokemon::incAcc(int x){
//    if (pokeacc < 6){
//        pokeacc += x;
//    }
//}
//
//void Pokemon::decAcc(int x){
//    if (pokeacc > -6){
//        pokeacc -= x;
//    }
//}

//int Pokemon::getEva() {
//    return evasion;
//}

double Pokemon::getEvaStage() {
	return statEva[0];
}

//void Pokemon::setEva(int x) {
//    evasion = x;
//}

void Pokemon::setEvaStage(int x) {
	statEva[0] += x;
	if (statEva[0] > 6) {
		statEva[0] = 6;
	}
	else if (statEva[0] < -6) {
		statEva[0] = -6;
	}
}

//void Pokemon::incEva(int x) {
//    if (evasion < 6) {
//        evasion += x;
//    }
//}
//
//void Pokemon::decEva(int x) {
//    if (evasion > 6) {
//        evasion -= x;
//    }
//}
