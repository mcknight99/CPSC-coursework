#ifndef PALINDROME_MODULE_H
#define PALINDROME_MODULE_H

#include <vector>

// Function to check if the palindrome constraint is satisfied
bool isPalindromeSafe(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{
    // Cast the data pointer to the appropriate type
    std::vector<std::vector<std::pair<int, int>>> *palindromeData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);

    // Check each palindrome for safety
    for (const auto &palindrome : *palindromeData)
    {
        int n = palindrome.size();
        for (int i = 0; i < n / 2; ++i) //"ignores" the middle element if n is odd
        {
            int r1 = palindrome[i].first; // current cell of the palindrome
            int c1 = palindrome[i].second;
            int r2 = palindrome[n - i - 1].first; // mirrored cell on the palindrome
            int c2 = palindrome[n - i - 1].second;

            if ((row == r1 && col == c1 || row == r2 && col == c2) && board[r2][c2] != 0 && num != board[r2][c2])
                return false;
            // If the current cell is the passed in cell and the values are different and the compared cell is nonzero, return false

            if ((row == r2 && col == c2 || row == r1 && col == c1) && board[r1][c1] != 0 && num != board[r1][c1])
                return false;
            // If the mirrored cell is the passed in cell and the values are different and the compared cell is nonzero, return false
        }
    }
    return true; // All palindromes are safe
}

std::vector<int> getPalindromeLogicalNums(const std::vector<std::vector<int>> &board, int row, int col, void *data)
{
    std::vector<int> logicalNums;
    std::vector<std::vector<std::pair<int, int>>> *palindromeData = static_cast<std::vector<std::vector<std::pair<int, int>>> *>(data);

    for (int i = 1; i <= 9; i++)
    {
        if (isPalindromeSafe(board, row, col, i, data))
        {
            logicalNums.push_back(i);
        }
    }

    return logicalNums;
}

#endif // PALINDROME_MODULE_H