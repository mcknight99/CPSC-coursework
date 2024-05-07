#ifndef ANTIKING_H
#define ANTIKING_H

#include <vector>

// Function to check if the "antiking" constraint is satisfied
bool isAntikingSafe(const std::vector<std::vector<int>>& board, int row, int col, int num, void* data) {
    static const std::vector<std::pair<int, int>> kingMoves = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    // Check each cell reachable by a king's move
    for (const auto& move : kingMoves) {
        int newRow = row + move.first;
        int newCol = col + move.second;
        // Check if the new position is within the board bounds
        if (newRow >= 0 && newRow < 9 && newCol >= 0 && newCol < 9) {
            // Check if the cell contains the same digit
            if (board[newRow][newCol] == num) {
                return false; // Antiking constraint violated
            }
        }
    }
    return true; // Antiking constraint satisfied
}

#endif /* ANTIKING_H */
