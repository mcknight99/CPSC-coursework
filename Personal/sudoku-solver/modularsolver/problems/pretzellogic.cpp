// https://sudokupad.app/s2778hgmzt pretzel logic (cages + antiknight) heartburn
// "Pretzel Logic" by FullDeck and Missing a Few Cards
// Antiknight, cages; heartburn
// Takes 50 seconds to run on my machine, so give it a second (and 49 more)

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/antiknightModule.h"
#include "../modules/cageModule.h"

#include <chrono>

using namespace std;

int main()
{

    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {4, 0, 0, 0, 0, 0, 0, 0, 5},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 8, 0, 0, 0},
        {0, 0, 4, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 3, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 0, 0},
        {0, 0, 0, 9, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {9, 0, 0, 0, 0, 0, 0, 0, 6}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{1, 1}, {1, 2}, {2, 1}}, 15},
        {{{1, 3}, {1, 4}, {1, 5}, {2, 4}}, 21},
        {{{1, 6}, {1, 7}, {2, 7}}, 15},
        {{{3, 1}, {4, 1}, {5, 1}, {4, 2}}, 11},
        {{{4, 4}, {3, 4}, {4, 3}, {5, 4}, {4, 5}}, 16},
        {{{4, 6}, {4, 7}, {3, 7}, {5, 7}}, 29},
        {{{7, 1}, {7, 2}, {6, 1}}, 15},
        {{{7, 3}, {7, 4}, {7, 5}, {6, 4}}, 19},
        {{{7, 7}, {7, 6}, {6, 7}}, 15}};

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
    cagesConstraint.data = &cagesData;
    constraints.push_back(cagesConstraint);

    // Add antiknight constraint
    Constraint antiknightConstraint;
    antiknightConstraint.check = isAntiknightSafe;
    antiknightConstraint.data = nullptr;
    constraints.push_back(antiknightConstraint);

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