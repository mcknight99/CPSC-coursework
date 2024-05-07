// https://sudokupad.app/6ijzpd1jdd
// "No Marks" by FullDeck and Missing a Few Cards
// Cages; Brassica

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/cageModule.h"

#include <chrono>

using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // Define the Sudoku board
    vector<vector<int>> board = {
        {8, 0, 0, 0, 0, 0, 0, 0, 3},
        {0, 0, 7, 0, 1, 0, 6, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 8, 3, 0, 4, 5, 0, 2},
        {0, 5, 0, 0, 9, 0, 0, 3, 0},
        {4, 0, 3, 5, 0, 6, 8, 0, 9},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 4, 0, 2, 0, 0},
        {9, 0, 0, 0, 0, 0, 0, 0, 6}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{0, 2}, {0, 3}}, 16}, // Cells (0,2), and (0,3) form a cage with sum 16
        {{{0, 5}, {0, 6}}, 3},
        {{{1, 1}, {1, 2}}, 10},
        {{{1, 6}, {1, 7}}, 8},
        {{{2, 0}, {2, 1}}, 3},
        {{{2, 3}, {3, 3}}, 7},
        {{{2, 5}, {3, 5}}, 9},
        {{{2, 7}, {2, 8}}, 17},
        {{{5, 3}, {6, 3}}, 11},
        {{{5, 5}, {6, 5}}, 13},
        {{{6, 0}, {6, 1}}, 5},
        {{{6, 7}, {6, 8}}, 5},
        {{{7, 1}, {7, 2}}, 7},
        {{{7, 6}, {7, 7}}, 10},
        {{{8, 2}, {8, 3}}, 6},
        {{{8, 5}, {8, 6}}, 4}};

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
        printBoard(board);
    }
    else
    {
        cout << "No solution exists!\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
}