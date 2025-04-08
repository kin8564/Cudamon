#include <string>
#include <array>
#include "types.h"
#include "Move.h"

class Move {
    private:
        std::string name;
        Types type;
        int category;
        int power, accuracy, pp;
        std::array<int, 8> statEffect; // {buff/debuff, atk, def, spa, spd, spe, acc, eva}

    public:
        Move (std::string name, Types type, int cat, int pw, int acc, int points, std::array<int, 8> statArray)
                : name(name), type(type), category(cat), power(pw), accuracy(acc), pp(points), statEffect(statArray) {}

        std::string getName() {
            return name;
        }

        Types getType() {
            return type;
        }

        int getCat() {
            return category;
        }

        int getPow() {
            return power;
        }

        int getAcc() {
            return accuracy;
        }

        int getPP() {
            return pp;
        }

        std::array<int, 8> getEff() {
            return statEffect;
        }
};