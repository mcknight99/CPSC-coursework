#ifndef THERMOMETER_MODULE_H
#define THERMOMETER_MODULE_H

#include <vector>

// std::vector<std::vector<std::pair<int, int>>> *thermometerData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);
//                     ^ vector of cells (r,c) in a thermometer
//        ^ list of thermometers in a problem 

// Function to check if a thermometer constraint is satisfied with strictly increasing numbers
bool isThermometersSafe_StrictlyIncreasing(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{
    // Cast the data pointer to the appropriate type
    std::vector<std::vector<std::pair<int, int>>> *thermometerData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);

    // Check each thermometer for safety
    for (const auto &thermometer : *thermometerData)
    {
        int prevNum = -1; // Initialize previous number to an invalid value
        for (const auto &cell : thermometer)
        {
            int r = cell.first;
            int c = cell.second;
            int currentNum = board[r][c];
            if (currentNum != 0)
            {
                if (prevNum != -1 && currentNum <= prevNum)
                {
                    return false; // Thermometer is not strictly increasing
                }
                prevNum = currentNum;
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
        int prevNum = -1; // Initialize previous number to an invalid value
        for (const auto &cell : thermometer)
        {
            int r = cell.first;
            int c = cell.second;
            int currentNum = board[r][c];
            if (currentNum != 0)
            {
                if (prevNum != -1 && currentNum < prevNum)
                {
                    return false; // Thermometer is not non-decreasing
                }
                prevNum = currentNum;
            }
        }
    }
    return true; // All thermometers are safe (non-decreasing)
}


#endif