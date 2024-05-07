#ifndef CAGE_MODULE_H
#define CAGE_MODULE_H

#include <vector>
#include <set>

// vector<pair<vector<pair<int, int>>, int>>* cagesData = static_cast<vector<pair<vector<pair<int, int>>, int>>*>(data);
//                   ^ vector of cells (r,c) in a cage
//                                       ^ sum of the cage
//           ^ pairing the list of cells with the sum of the cage (cage.first is the list of cells, cage.second is the sum of the cage)
//     ^ vector of cages to be checked
// This helped me make sense of it: V{p<v{p<i,i>},i>}

// Function to check if a cages constraint is satisfied
bool isCagesSafe(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{
    // Cast the data pointer to the appropriate type
    // cout<<"row: "<<row<<" col: "<<col<<" num: "<<num<<"\n";
    std::vector<std::pair<std::vector<std::pair<int, int>>, int>> *cagesData = static_cast<std::vector<std::pair<std::vector<std::pair<int, int>>, int>> *>(data);

    // Check each cage for safety
    for (const auto &cage : *cagesData)
    {
        int sum = 0;
        std::set<int> usedDigits; // Set to track unique digits within the cage
        bool filled = true;  // Flag to track if all cells in the cage are filled
        for (const auto &cell : cage.first)
        {
            int r = cell.first;
            int c = cell.second;
            int currentNum = board[r][c];
            if (currentNum == 0)
            {
                filled = false; // Cage is not full
            }
            else
            {
                // Check if the digit is repeated within the cage
                if (usedDigits.count(currentNum) > 0)
                {
                    return false; // Digit is repeated within the cage
                }
                usedDigits.insert(currentNum);
                sum += currentNum;
            }
        }
        // Check if all digits are unique and the sum matches the cage sum
        if ((filled && sum != cage.second) || (filled && usedDigits.size() != cage.first.size()))
        {
            return false; // Cage is not safe
        }
    }
    return true; // All cages are safe
}

#endif