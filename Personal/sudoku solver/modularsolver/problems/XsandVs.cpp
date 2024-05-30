// https://sudokupad.app/2n1c374516
// "Xs and Vs" by bitmask
// Antiking // XV Sums

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/antikingModule.h"
#include "../modules/xvsumModule.h"

#include <chrono>

using namespace std;

int main()
{

    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {0, 0, 0, 4, 0, 8, 5, 0, 0},
        {9, 0, 0, 0, 0, 0, 0, 0, 7},
        {4, 1, 0, 0, 0, 9, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 6, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 8, 0},
        {0, 0, 0, 0, 7, 0, 0, 0, 0},
        {5, 0, 0, 0, 0, 0, 0, 0, 8},
        {0, 0, 0, 0, 6, 0, 0, 0, 0},
        {0, 6, 0, 0, 0, 1, 0, 7, 0}};

    vector<tuple<int, int, int, int, char>> XVSumData = {
        {1, 2, 1, 3, 'V'},
        {1, 5, 1, 6, 'X'},
        {2, 1, 3, 1, 'X'},
        {3, 2, 3, 3, 'V'},
        {2, 7, 3, 7, 'V'},
        {4, 5, 4, 6, 'X'},
        {5, 7, 6, 7, 'X'},
        {5, 1, 6, 1, 'V'},
        {6, 2, 6, 3, 'X'},
        {6, 5, 6, 6, 'V'},
        {7, 2, 8, 2, 'X'},
        {7, 6, 8, 6, 'V'}};

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add antiking constraint
    Constraint antikingConstraint;
    antikingConstraint.check = isAntikingSafe;
    antikingConstraint.data = nullptr;
    constraints.push_back(antikingConstraint);

    // Add XV Sum constraint
    Constraint XVSumConstraint;
    XVSumConstraint.check = isXVSafe;
    XVSumConstraint.data = &XVSumData;
    constraints.push_back(XVSumConstraint);

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