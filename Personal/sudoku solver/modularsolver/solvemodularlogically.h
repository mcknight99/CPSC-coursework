#ifndef SOLVEMODULARLOGICALLY_H
#define SOLVEMODULARLOGICALLY_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

#include <stdlib.h>

bool print = false;

// Structure for modular constraint
struct Constraint
{
    std::vector<int> (*check)(const std::vector<std::vector<int>> &, int, int, void *);
    void *data;
};

struct Step
{
    int row;
    int col;
    int value;
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

// This implementation is designed to determine if a Sudoku board can be solved logically and without guessing
// Should also provide if the board has several or no solutions
// the function returns a list of steps from the initial board to the solved board as logical steps
// The board is passed by reference, so the changes are reflected in the original board
// The solver utilizes module logical num functions to determine the possible numbers for each cell, makes a list of possible nums in each cell based off of and-ing the logical nums from each module, and prioritizing the cells with the least possible nums in each step of the solving process
std::vector<Step> solveModularLogically(std::vector<std::vector<int>> &board, const std::vector<Constraint> &constraints)
{
    std::vector<std::vector<std::vector<int>>> logicalBoard(9, std::vector<std::vector<int>>(9));
    std::vector<std::vector<std::vector<int>>> logicalBoardPrevious(9, std::vector<std::vector<int>>(9));
    std::vector<Step> steps;
    int filledCells = 0;
    while (filledCells < 81)
    {
        filledCells = 0;
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
                    std::vector<int> logicalNums;
                    std::map<int, int> logicalNumsMap;

                    // Check if it's safe to place 'num' at the current position
                    // Make sure 'num' is safe for all constraints and only push back if it is
                    for (const auto &constraint : constraints)
                    {
                        std::vector<int> constraintLogicalNums = constraint.check(board, row, col, constraint.data);
                        for (int logicalNum : constraintLogicalNums)
                        {
                            logicalNumsMap[logicalNum]++;
                        }
                    }

                    for (const auto &logicalNum : logicalNumsMap)
                    {
                        if (logicalNum.second == constraints.size())
                        {
                            logicalNums.push_back(logicalNum.first);
                        }
                    }
                    
                    logicalBoard[row][col] = logicalNums;

                    if (logicalNums.size() == 1)
                    {
                        board[row][col] = logicalNums[0];
                        Step step;
                        step.row = row;
                        step.col = col;
                        step.value = logicalNums[0];
                        steps.push_back(step);
                    }
                    else if (logicalNums.size() == 0)
                    {
                        return steps;
                    }
                }
                else
                {
                    filledCells++;
                }
            }
        }

        // Find the cells within the logical board where there is a unique logical placement in that cell, row, or column
        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                if (logicalBoard[row][col].size() > 1)
                {
                    std::vector<int> rowLogicalNums;
                    std::vector<int> colLogicalNums;
                    for(int i = 0; i<9; i++) {
                        for(int logicalNum : logicalBoard[row][i]) {
                            rowLogicalNums.push_back(logicalNum);
                        }
                        for(int logicalNum : logicalBoard[i][col]) {
                            colLogicalNums.push_back(logicalNum);
                        }
                    }
                    std::vector<int> uniqueLogicalNums;
                    for (int logicalNum : logicalBoard[row][col])
                    {
                        if (std::find(rowLogicalNums.begin(), rowLogicalNums.end(), logicalNum) == rowLogicalNums.end() && std::find(colLogicalNums.begin(), colLogicalNums.end(), logicalNum) == colLogicalNums.end())
                        {
                            uniqueLogicalNums.push_back(logicalNum);
                        }
                    }
                    if (uniqueLogicalNums.size() == 1)
                    {
                        board[row][col] = uniqueLogicalNums[0];
                        Step step;
                        step.row = row;
                        step.col = col;
                        step.value = uniqueLogicalNums[0];
                        steps.push_back(step);
                    }
                }
            }
        }
        if(logicalBoard == logicalBoardPrevious) {
            return steps;
        }
        logicalBoardPrevious = logicalBoard;
    }
    return steps;
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

// Function to get the logical numbers for a given position
std::vector<int> getLogicalNums(const std::vector<std::vector<int>> &board, int row, int col, void *data)
{
    std::vector<int> logicalNums;

    for (int i = 1; i <= 9; i++)
    {
        if (isSafe(board, row, col, i, data))
        {
            logicalNums.push_back(i);
        }
    }

    return logicalNums;
}

#endif // SOLVEMODULARLOGICALLY_H