/** @file */
#include "grlib/utility.hpp"

int randomize(int min,int max)
{
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min,max);

        return dis(gen);
}

