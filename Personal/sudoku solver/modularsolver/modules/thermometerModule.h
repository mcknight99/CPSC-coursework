#ifndef THERMOMETER_MODULE_H
#define THERMOMETER_MODULE_H

#include <vector>

// std::vector<std::vector<std::pair<int, int>>> *thermometerData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);
//                     ^ vector of cells (r,c) in a thermometer
//        ^ list of thermometers in a problem

// had to fix a bug where if the last cell in a thermometer to be finished was a bulb, it would return safe because the prevNum was -1
// Somehow the new implementation now works faster than the previous implementation, when I thought it would be slower.
// Thanks to A Slow Walk Around the Quad for the edge case I guess lol

// Function to check if a thermometer constraint is satisfied with strictly increasing numbers
bool isThermometersSafe_StrictlyIncreasing(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{
    // Cast the data pointer to the appropriate type
    std::vector<std::vector<std::pair<int, int>>> *thermometerData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);

    // Check each thermometer for safety
    for (const auto &thermometer : *thermometerData)
    {
        bool cellInThermo = false;
        for (const auto &cell : thermometer)
        {
            if (cell.first == row && cell.second == col)
            {
                cellInThermo = true; // The cell is part of a thermometer
            }
        }
        if (!cellInThermo)
        {
            continue; // Skip the thermometer if the cell is not part of it
        }

        int lastCell = -1;
        int thisCell;
        int nextCell;
        for (int i = 0; i < thermometer.size(); ++i)
        {
            int r = thermometer.at(i).first;
            int c = thermometer.at(i).second;
            if (r == row && c == col)
            {
                thisCell = num;
            }
            else if (board[r][c] != 0)
            {
                thisCell = board[r][c];
            }
            else
            {
                continue; // Skip the cell if it is empty
            }
            if (i == thermometer.size() - 1)
            {
                nextCell = -1;
            }
            else
            {
                int j = i + 1;
                while (j < thermometer.size() && board[thermometer.at(j).first][thermometer.at(j).second] == 0)
                {
                    ++j;
                }
                if (j == thermometer.size())
                {
                    // Reached the end of thermometer with no next value
                    nextCell = -1;
                }
                else
                {
                    // Scan ahead to the next nonzero value, because this cell should still be less than the next nonzero value
                    nextCell = board[thermometer.at(j).first][thermometer.at(j).second];
                }
            }

            // using lastCell and nextCell > 0 to test if it isn't -1 or 0
            //  if the last cell not is less than the current cell, or the next cell is not greater than the current cell, it is not strictly increasing

            if (thisCell != 0)
            {
                if ((lastCell > 0 && !(lastCell < thisCell)) || (nextCell > 0 && !(nextCell > thisCell)))
                {
                    return false; // Thermometer is not strictly increasing
                }

                lastCell = thisCell;
                // Maintain the previous nonzero value, because this (next) cell should still be greater than the previous nonzero value
            }
        }
    }
    return true; // All thermometers are safe (strictly increasing)
}

// Function to check if a thermometer constraint is satisfied with non-decreasing numbers
bool isThermometersSafe_Nondecreasing(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{
    // Cast the data pointer to the appropriate type
    std::vector<std::vector<std::pair<int, int>>> *thermometerData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);

    // Check each thermometer for safety
    for (const auto &thermometer : *thermometerData)
    {
        bool cellInThermo = false;
        for (const auto &cell : thermometer)
        {
            if (cell.first == row && cell.second == col)
            {
                cellInThermo = true; // The cell is part of a thermometer
            }
        }
        if (!cellInThermo)
        {
            continue; // Skip the thermometer if the cell is not part of it
        }

        int lastCell = -1;
        int thisCell;
        int nextCell;
        for (int i = 0; i < thermometer.size(); ++i)
        {
            int r = thermometer.at(i).first;
            int c = thermometer.at(i).second;
            if (r == row && c == col)
            {
                thisCell = num;
            }
            else if (board[r][c] != 0)
            {
                thisCell = board[r][c];
            }
            else
            {
                continue; // Skip the cell if it is empty
            }
            if (i == thermometer.size() - 1)
            {
                nextCell = -1;
            }
            else
            {
                int j = i + 1;
                while (j < thermometer.size() && board[thermometer.at(j).first][thermometer.at(j).second] == 0)
                {
                    ++j;
                }
                if (j == thermometer.size())
                {
                    // Reached the end of thermometer with no next value
                    nextCell = -1;
                }
                else
                {
                    // Scan ahead to the next nonzero value, because this cell should still be less than or equal to the next nonzero value
                    nextCell = board[thermometer.at(j).first][thermometer.at(j).second];
                }
            }

            // using lastCell and nextCell > 0 to test if it isn't -1 or 0
            //  if the last cell not is less than or equal to the current cell, or the next cell is not greater than or equal to the current cell, it is not nondecreasing

            if (thisCell != 0)
            {
                if ((lastCell > 0 && !(lastCell <= thisCell)) || (nextCell > 0 && !(nextCell >= thisCell)))
                {
                    return false; // Thermometer is not nondecreasing
                }

                lastCell = thisCell;
                // Maintain the previous nonzero value, because this (next) cell should still be greater than or equal to the previous nonzero value
            }
        }
    }
    return true; // All thermometers are safe (nondecreasing)
}

std::vector<int> getThermometersLogicalNums_StrictlyIncreasing(const std::vector<std::vector<int>> &board, int row, int col, void *data)
{
    std::vector<int> logicalNums;
    std::vector<std::vector<std::pair<int, int>>> *thermometerData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);

    for (int i = 1; i <= 9; i++)
    {
        if (isThermometersSafe_StrictlyIncreasing(board, row, col, i, data))
        {
            logicalNums.push_back(i);
        }
    }

    return logicalNums;
}

std::vector<int> getThermometersLogicalNums_Nondecreasing(const std::vector<std::vector<int>> &board, int row, int col, void *data)
{
    std::vector<int> logicalNums;
    std::vector<std::vector<std::pair<int, int>>> *thermometerData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);

    for (int i = 1; i <= 9; i++)
    {
        if (isThermometersSafe_Nondecreasing(board, row, col, i, data))
        {
            logicalNums.push_back(i);
        }
    }

    return logicalNums;
}

#endif // THERMOMETER_MODULE_H