#ifndef SOLVEMODULARSUDOKU_H
#define SOLVEMODULARSUDOKU_H

#include <vector>
#include <iostream>

#include <stdlib.h>
// Uncomment out the print board function in solveModularSudoku and system("CLS") in solveModularSudoku() to see magic
// Warning, this makes the program run SIGNIFICANTLY slower (ex. An Easy Day's Knight ~18ms -> 47s) due to the number of console calls
// Therefore I would only use this as a fun, miscellaneous feature on quick solves if you are interested in seeing the method of solving the board
bool print = false;
// Some good ones to see with this feature:
// An Easy Day's Knight
// X's and V's 
// No Marks

// Structure for modular constraint
struct Constraint
{
    bool (*check)(const std::vector<std::vector<int>> &, int, int, int, void *);
    void *data;
};

// Function to print the Sudoku board
void printBoard(const std::vector<std::vector<int>> &board)
{
    for (int i = 0; i < 9; ++i)
    {
        if (i % 3 == 0 && i != 0)
        {
            std::cout << "---------------------\n";
        }
        for (int j = 0; j < 9; ++j)
        {
            if (j % 3 == 0 && j != 0)
            {
                std::cout << "| ";
            }
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// This is only using the single sudoku solver, not the solve several implementation
// The board is passed by reference, so the changes are reflected in the original board
// Once a solution is found, the function returns true and the solved board is passed up the call stack
bool solveModularSudoku(std::vector<std::vector<int>> &board, const std::vector<Constraint> &constraints)
{
    if (print)
    {
        // Clear the console
        system("CLS");
        printBoard(board);
    }
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            // Find empty cell
            if (board[row][col] == 0)
            {
                // Try placing numbers 1-9
                for (int num = 1; num <= 9; ++num)
                {
                    // Check if it's safe to place 'num' at the current position
                    bool safe = true;
                    for (const auto &constraint : constraints)
                    {
                        if (!(constraint.check(board, row, col, num, constraint.data)))
                        {
                            safe = false;
                            break;
                        }
                    }
                    if (safe)
                    {
                        // Place the number
                        board[row][col] = num;

                        // Recursively solve the rest of the puzzle
                        if (solveModularSudoku(board, constraints))
                        {
                            return true;
                        }

                        // If placing 'num' doesn't lead to a solution, backtrack
                        board[row][col] = 0;
                    }
                }
                // If no number can be placed, return false
                return false;
            }
        }
    }
    // If all cells are filled, return true
    return true;
}

// Function to check if it's safe to place a number at a given position
bool isSafe(const std::vector<std::vector<int>> &board, int row, int col, int num, void *data)
{
    // Basic Sudoku safety check
    for (int i = 0; i < 9; ++i)
    {
        if (board[row][i] == num || board[i][col] == num || board[(row - (row % 3)) + (i / 3)][(col - (col % 3)) + (i % 3)] == num)
        {
            return false;
        }
    }
    return true;
}

#endif // SOLVEMODULARSUDOKU_H