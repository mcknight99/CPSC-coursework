#ifndef XVSUM_MODULE_H
#define XVSUM_MODULE_H

#include <vector>
#include <tuple>

#include <iostream>

// Function to check if the XV Sum constraint is satisfied
bool isXVSafe(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{
    // Cast the data pointer to the appropriate type
    std::vector<std::tuple<int, int, int, int, char>> *XVSumData = static_cast<std::vector<std::tuple<int, int, int, int, char>> *>(data);

    // std::cout << "Checking kropki safety for " << row << " " << col << " " << num << std::endl;

    // Check each kropki pair for safety
    for (const auto &pair : *XVSumData)
    {
        int row1 = std::get<0>(pair);
        int col1 = std::get<1>(pair);
        int row2 = std::get<2>(pair);
        int col2 = std::get<3>(pair);
        char type = std::get<4>(pair);

        // Skip the current cell if it is not part of the kropki pairs
        if ((row == row1 && col == col1) || (row == row2 && col == col2))
        {
            // std::cout << "\tChecking pair " << row1 << " " << col1 << " " << row2 << " " << col2 << " " << type << std::endl;

            // Get the value of the other cell in the kropki pair
            int otherCellValue = (row == row1 && col == col1) ? board[row2][col2] : board[row1][col1];
            // Is also "satisfied" if the other cell is 0
            if (otherCellValue != 0)
            {
                if ((type == 'X' && num + otherCellValue == 10) ||
                    (type == 'V' && num + otherCellValue == 5))
                {
                    return true; // XV Sum constraint satisfied
                }
                else
                {
                    return false; // Kropki constraint violated
                }
            }
        }
    }
    return true; // XV Sum constraint satisfied if both cells are 0
}

#endif /* KROPKI_H */
