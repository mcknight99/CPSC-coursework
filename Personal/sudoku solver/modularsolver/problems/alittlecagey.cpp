// https://sudokupad.app/vmwl2wcedr
// "A Little Cagey" by FullDeck and Missing a Few Cards
// Cages; Indigestion
// This 1337 seconds (22 min 17 sec) to run on my machine, so be VERY patient.

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/cageModule.h"

#include <chrono>

using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {0, 0, 0, 0, 3, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {5, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 6, 0, 0, 0, 0}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{0, 1}, {1, 0}, {1, 1}}, 15},
        {{{1, 2}, {2, 1}, {2, 2}}, 12},
        {{{2, 3}, {3, 2}, {3, 3}}, 6},
        {{{0, 8}, {0, 7}, {1, 8}}, 15},
        {{{1, 7}, {1, 6}, {2, 7}}, 10},
        {{{2, 6}, {2, 5}, {3, 6}}, 7},
        {{{3, 5}, {3, 4}, {4, 5}}, 24},
        {{{8, 0}, {8, 1}, {7, 0}}, 16},
        {{{7, 1}, {7, 2}, {6, 1}}, 12},
        {{{6, 2}, {6, 3}, {5, 2}}, 7},
        {{{5, 3}, {5, 4}, {4, 3}}, 15},
        {{{8, 7}, {7, 8}, {7, 7}}, 13},
        {{{7, 6}, {6, 7}, {6, 6}}, 14},
        {{{6, 5}, {5, 6}, {5, 5}}, 6}};

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
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken: " << duration.count() << " seconds" << endl;
}