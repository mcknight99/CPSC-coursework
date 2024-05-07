// https://sudokupad.app/0qqqkdih3s blast off (cages + antiknight) indigestion
// "Blast Off" by FullDeck and Missing a Few Cards
// Antiknight, cages; indigestion

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
        {3, 0, 0, 0, 0, 0, 0, 0, 7},
        {0, 0, 0, 0, 5, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {6, 0, 0, 0, 0, 0, 0, 0, 4},
        {0, 0, 0, 0, 8, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{0, 1}, {0, 2}, {0, 3}, {1, 1}}, 13},
        {{{0, 5}, {0, 6}, {0, 7}, {1, 7}}, 28},
        {{{2, 2}, {1, 2}, {2, 3}}, 14},
        {{{2, 5}, {2, 6}, {1, 6}}, 19},
        {{{3, 2}, {3, 3}, {4, 3}}, 22},
        {{{3, 5}, {3, 6}, {4, 5}}, 10},
        {{{5, 2}, {5, 3}, {6, 2}}, 21},
        {{{5, 5}, {5, 6}, {6, 6}}, 8},
        {{{7, 1}, {7, 2}, {7, 3}, {6, 3}}, 24},
        {{{7, 5}, {7, 6}, {7, 7}, {6, 5}}, 14},
        {{{7, 0}, {8, 0}}, 7},
        {{{8, 8}, {7, 8}}, 8}};

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
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
}