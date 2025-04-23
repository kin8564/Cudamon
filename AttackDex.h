#ifndef AttackDex_H
#define AttackDex_H

#include <iostream>
#include <unordered_map>
#include <string>
#include "Move.h"

class AttackDex{
    public:
        std::unordered_map<std::string, Move> attackdex;

        void populate();

};

#endif
