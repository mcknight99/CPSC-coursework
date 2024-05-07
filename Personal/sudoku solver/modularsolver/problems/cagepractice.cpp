// https://sudokupad.app/ek2cvdxj8h
//  "Cage Practice" by FullDeck and Missing a Few Cards

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/cageModule.h"

using namespace std;

int main()
{
    // Define the Sudoku board
    vector<vector<int>> board = {
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{0, 0}, {0, 1}, {0, 2}}, 6}, // Squares in the cage (0,0), (0,1), (0,2) must sum to 6
        {{{0, 3}, {1, 3}, {2, 3}}, 7},
        {{{0, 5}, {0, 6}, {1, 5}}, 21},
        {{{1, 1}, {2, 1}, {3, 1}}, 20},
        {{{1, 7}, {2, 7}}, 12},
        {{{2, 5}, {2, 6}}, 5},
        {{{3, 2}, {4, 2}}, 13},
        {{{3, 6}, {3, 7}}, 16},
        {{{4, 0}, {4, 1}}, 17},
        {{{4, 3}, {4, 4}, {4, 5}}, 15},
        {{{4, 6}, {5, 6}}, 7},
        {{{4, 7}, {4, 8}}, 3},
        {{{5, 1}, {5, 2}}, 4},
        {{{5, 7}, {6, 7}, {7, 7}}, 10},
        {{{6, 1}, {7, 1}}, 8},
        {{{6, 2}, {6, 3}}, 15},
        {{{6, 5}, {7, 5}, {8, 5}}, 23},
        {{{7, 3}, {8, 3}, {8, 2}}, 9},
        {{{8, 6}, {8, 7}, {8, 8}}, 24}};

    // Define constraints
    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add cages constraint
    Constraint cagesConstraint;
    cagesConstraint.check = isCagesSafe;
    // cagesConstraint.data = nullptr; // Replace with actual cages data
    cagesConstraint.data = &cagesData;
    constraints.push_back(cagesConstraint);

    // Solve the Sudoku puzzle
    if (solveModularSudoku(board, constraints))
    {
        cout << "Solution:\n";
        // Print the solution
        printBoard(board);
    }
    else
    {
        cout << "No solution exists!\n";
    }

    return 0;
}