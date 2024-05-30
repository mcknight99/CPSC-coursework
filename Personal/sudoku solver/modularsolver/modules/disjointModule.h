#ifndef DISJOINT_MODULE_H
#define DISJOINT_MODULE_H

#include <vector>

// Function to check if the disjoint groups constraint is satisfied
bool isDisjointSafe(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{
    // Check if the cell is in a disjoint group (same position in any 3x3)
    for (int i = 0; i < 9; ++i)
    {
        if (board[(row % 3) + (3 * (i / 3))][(col % 3) + (3 * (i % 3))] == num)
        {
            return false;
        }
    }
    return true;
}

#endif /* DISJOINT_H */
