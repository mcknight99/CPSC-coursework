#ifndef GENERATE_H
#define GENERATE_H

#include "vars.h"

Board generate(int r, int c, float density, std::string seed);
Board generate(int r, int c, float density, int seed = 0);

// Validate the generated board
// Return true if the board is a valid state
// If testComplete is true, also check if the board is completely filled
bool validate(Board b, bool testComplete = false);

#endif