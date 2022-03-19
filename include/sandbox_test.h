#ifndef TEST_H
#define TEST_H

#include "dustengine.h"

typedef enum {
    STEAM,
    SMOKE,
    WATER,
    SAND,
    ANTI_SAND,
    LAVA,
    STONE,
    BRICK,
    NUM_TYPES
} TEST_TYPES;

DustType* Test_createConfig(int, int);
void Test_destroyConfig(DustType*, int);

#endif //TEST_H