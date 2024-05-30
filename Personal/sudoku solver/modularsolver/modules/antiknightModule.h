#ifndef ANTIKNIGHT_H
#define ANTIKNIGHT_H

#include <vector>

// Function to check if the antiknight constraint is satisfied
bool isAntiknightSafe(const std::vector<std::vector<int>>& board, int row, int col, int num, void* data) {
    static const std::vector<std::pair<int, int>> knightMoves = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    // Check each cell reachable by a knight's move
    for (const auto& move : knightMoves) {
        int newRow = row + move.first;
        int newCol = col + move.second;
        // Check if the new position is within the board bounds
        if (newRow >= 0 && newRow < 9 && newCol >= 0 && newCol < 9) {
            // Check if the cell contains the same digit
            if (board[newRow][newCol] == num) {
                return false; // Antiknight constraint violated
            }
        }
    }
    return true; // Antiknight constraint satisfied
}
#endif /* ANTIKNIGHT_H */
