#include <string>
#include <array>
#include <cstring>
#include "Types.h"
#include "Move.h"

char name[30];
TYPES type;
int category;
int power, accuracy, pp;
int statEffect[8]; // {buff/debuff, atk, def, spa, spd, spe, acc, eva}

Move::Move(const char* n, TYPES type, int cat, int pw, int acc, int points, int statArray[8])
        : type(type), category(cat), power(pw), accuracy(acc), pp(points) {
        strncpy(name, n, sizeof(name));
        name[sizeof(name) - 1] = '\0'; // Null-terminate just in case
        for (int j = 0; j < 8; ++j) statEffect[j] = statArray[j];
        }

Move::Move() : type(TYPES::NONE), category(0), power(0), accuracy(0), pp(0) {
    name[0] = '\0'; // empty string
    for (int j = 0; j < 8; ++j) {
        statEffect[j] = 0;
    }
}

char* Move::getName() {
    return name;
}

TYPES Move::getType() {
    return type;
}

int Move::getCat() {
    return category;
}

int Move::getPow() {
    return power;
}

int Move::getAcc() {
    return accuracy;
}

int Move::getPP() {
    return pp;
}

int* Move::getEff() {
    return statEffect;
}