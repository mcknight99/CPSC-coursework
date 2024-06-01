// https://sudokupad.app/kiym0h0l87
// "Knight in the Hot Seat" by FullDeck and Missing a Few Cards
// Antiknight // Thermometers
// Takes about 35 seconds to run

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/antiknightModule.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 5, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 9, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 4},
        {0, 6, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 7, 0, 0}};

    vector<vector<pair<int, int>>> thermometerData = {
        {{1,4},{1,3},{2,3},{2,4},{2,5},{1,5}},
        {{2,6},{3,5}},
        {{4,7},{5,7},{5,6},{4,6},{3,6},{3,7}},
        {{7,4},{7,5},{6,5},{6,4},{6,3},{7,3}},
        {{4,1},{3,1},{3,2},{4,2},{5,2},{5,1}},
        {{2,2},{3,3}}
    };

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add thermometer constraint
    Constraint thermometerConstraint;
    thermometerConstraint.check = isThermometersSafe_StrictlyIncreasing;
    thermometerConstraint.data = &thermometerData;
    constraints.push_back(thermometerConstraint);

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
        cout << "No solution exists\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken: " << duration.count() << " seconds" << endl;

}