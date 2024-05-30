#ifndef QUADRUPLES_MODULE_H
#define QUADRUPLES_MODULE_H

#include <vector>
#include <algorithm>
#include <list>

// Function to check if the quadruples constraint is satisfied
bool isQuadruplesSafe(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{

    // Cast the data pointer to the appropriate type
    std::vector<std::pair<std::vector<std::pair<int, int>>, std::vector<int>>> *quadruplesData = static_cast<std::vector<std::pair<std::vector<std::pair<int, int>>, std::vector<int>>> *>(data);

    // Check each quadruple for safety
    for (const auto &quadruple : *quadruplesData)
    {
        // Get the cells and values from the quadruple
        const std::vector<std::pair<int, int>> &quadrupleCells = quadruple.first;

        bool cellFound = false;
        for (const auto &cell : quadrupleCells)
        {
            if (cell.first == row && cell.second == col)
            {
                // The cell is not part of the quadruple
                cellFound = true;
            }
        }
        if (!cellFound)
        {
            // Skip the quadruple to save some calculation time if the cell is not part of the quadruple
            continue;
        }

        std::vector<int> quadrupleValues = quadruple.second; 

        std::vector<int> usedValues;
        int wildCardCount = 0;
        for (const auto &cell : quadrupleCells)
        {
            // If the row and col being tested is part of the quadruple, add the num to the values vector instead of what's on the board
            if (cell.first == row && cell.second == col)
            {
                usedValues.push_back(num);
            }
            else
            {
                usedValues.push_back(board[cell.first][cell.second]);
            }
            // zeroes are important here, as they mark cells that can be future "wild cards" for the quadruple to be satisfied

            if (usedValues.back() == 0)
            {
                wildCardCount++;
            }
        }

        // Check if the quadruple constraint is satisfied
        // If the number of wild cards is greater than or equal to the remaining required values, then the constraint is satisfied
        for (auto it = usedValues.begin(); it != usedValues.end(); ++it)
        {
            auto found = std::find(quadrupleValues.begin(), quadrupleValues.end(), *it);
            if (found != quadrupleValues.end())
            {
                quadrupleValues.erase(found);
            }
        }

        if (quadrupleValues.size() > wildCardCount)
        {
            return false;
        }
    }

    // Quadruple constraint is satisfied
    return true;
}

#endif // QUADRUPLES_MODULE_H