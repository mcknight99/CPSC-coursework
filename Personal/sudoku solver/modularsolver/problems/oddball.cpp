// https://sudokupad.app/dvet4fjt9z
// "Oddball" by Adam
// Antiking // Antiknight // Kropki Pairs

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/antiknightModule.h"
#include "../modules/kropkiModule.h"
#include "../modules/antikingModule.h"

#include <chrono>

using namespace std;

int main()
{

    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 9, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 3, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 0, 0, 0, 5, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 7, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    vector<tuple<int, int, int, int, char>> kropkiData = {
        {0, 0, 0, 1, 'B'},
        {0, 0, 1, 0, 'B'},
        {1, 0, 2, 0, 'B'},
        {0, 4, 0, 5, 'B'},
        {1, 8, 2, 8, 'B'},
        {4, 0, 4, 1, 'B'},
        {4, 2, 5, 2, 'B'},
        {3, 5, 4, 5, 'B'},
        {5, 7, 6, 7, 'B'},
        {6, 1, 7, 1, 'B'},
        {8, 7, 8, 8, 'B'}};

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add antiknight constraint
    Constraint antiknightConstraint;
    antiknightConstraint.check = isAntiknightSafe;
    antiknightConstraint.data = nullptr;
    constraints.push_back(antiknightConstraint);

    // Add kropki constraint
    Constraint kropkiConstraint;
    kropkiConstraint.check = isKropkiSafe;
    kropkiConstraint.data = &kropkiData;
    constraints.push_back(kropkiConstraint);

    // Add antiking constraint
    Constraint antikingConstraint;
    antikingConstraint.check = isAntikingSafe;
    antikingConstraint.data = nullptr;
    constraints.push_back(antikingConstraint);

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