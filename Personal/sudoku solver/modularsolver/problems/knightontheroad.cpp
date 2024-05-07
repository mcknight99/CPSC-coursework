// https://sudokupad.app/kgp6jzmk37 knight on the road (cages + antiknight) gas
// "Knight on the Road" by FullDeck and Missing a Few Cards
// Antiknight, cages; gas

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
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{0, 0}, {0, 1}, {1, 0}}, 14},
        {{{1, 1}, {1, 2}, {2, 1}}, 20},
        {{{2, 2}, {3, 2}, {3, 3}}, 7},
        {{{0, 4}, {1, 4}}, 11},
        {{{2, 4}, {3, 4}}, 14},
        {{{0, 7}, {0, 8}, {1, 8}}, 15},
        {{{1, 6}, {1, 7}, {2, 7}}, 8},
        {{{2, 6}, {3, 6}, {3, 5}}, 23},
        {{{4, 0}, {4, 1}}, 7},
        {{{4, 2}, {4, 3}}, 8},
        {{{4, 5}, {4, 6}}, 12},
        {{{4, 7}, {4, 8}}, 12},
        {{{7, 0}, {8, 0}, {8, 1}}, 22},
        {{{6, 1}, {7, 1}, {7, 2}}, 6},
        {{{6, 2}, {5, 2}, {5, 3}}, 22},
        {{{5, 4}, {6, 4}}, 5},
        {{{7, 4}, {8, 4}}, 9},
        {{{8, 8}, {8, 7}, {7, 8}}, 10},
        {{{7, 7}, {7, 6}, {6, 7}}, 24},
        {{{5, 5}, {5, 6}, {6, 6}}, 8}};

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