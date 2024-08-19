// https://sudokupad.app/aevobgfx9o
// "King's Disease" by Roundout
// Antiking, Thermometers; indigestion

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/antikingModule.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main() {

    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {5, 0, 0, 0, 6, 0, 0, 0, 9},
        {0, 6, 0, 0, 0, 0, 0, 8, 0},
        {0, 0, 3, 4, 0, 2, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 5},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 7, 0, 1, 5, 0, 0},
        {0, 5, 0, 0, 0, 0, 0, 7, 0},
        {7, 0, 0, 0, 0, 0, 0, 0, 8}};

    vector<vector<pair<int, int>>> thermometerData = {
        {{2,2},{2,1},{2,0}},
        {{0,4},{1,4}},
        {{2,3},{3,3},{4,3},{5,3}},
        {{3,4},{4,4},{5,4}},
        {{2,5},{3,5},{4,5},{5,5}},
        {{2,6},{2,7},{2,8}},
        {{6,3},{7,2},{8,1}},
        {{6,5},{7,6},{8,7}}
        };

    // Define constraints
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

    // Add thermometers constraint
    Constraint thermometersConstraint;
    thermometersConstraint.check = isThermometersSafe_StrictlyIncreasing;
    thermometersConstraint.data = &thermometerData;
    constraints.push_back(thermometersConstraint);

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