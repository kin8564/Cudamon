#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <array>
#include "Types.h"  // Include Types enum class

class Move {
    private:
        std::string name;
        Types type;
        int category;
        int power, accuracy, pp;
        std::array<int, 8> statEffect; // {buff/debuff, atk, def, spa, spd, spe, acc, eva}

    public:
        Move (std::string name, Types type, int cat, int pw, int acc, int points, std::array<int, 8> statArray);
        std::string getName();
        Types getType();
        int getCat();
        int getPow();
        int getAcc();
        int getPP();
        std::array<int, 8> getEff();
};

#endif // MOVE_H
