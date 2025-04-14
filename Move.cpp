#include <string>
#include <array>
#include "Types.h"
#include "Move.h"

std::string name;
TYPES type;
int category;
int power, accuracy, pp;
std::array<int, 8> statEffect; // {buff/debuff, atk, def, spa, spd, spe, acc, eva}

Move::Move(std::string name, TYPES type, int cat, int pw, int acc, int points, std::array<int, 8> statArray)
        : name(name), type(type), category(cat), power(pw), accuracy(acc), pp(points), statEffect(statArray) {}

std::string Move::getName() {
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

std::array<int, 8> Move::getEff() {
    return statEffect;
}