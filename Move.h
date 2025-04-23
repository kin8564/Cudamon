#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <array>
#include "Types.h"  // Include Types enum class

struct Move {
        char name[30];
        TYPES type;
        int category;
        int power, accuracy, pp;
        int statEffect[8]; // {buff/debuff, atk, def, spa, spd, spe, acc, eva}

        Move(const char* name, TYPES type, int cat, int pw, int acc, int points, int statArray[8]);
        Move();
        char* getName();
        TYPES getType();
        int getCat();
        int getPow();
        int getAcc();
        int getPP();
        int* getEff();
};

#endif // MOVE_H
