// https://sudokupad.app/bptmo1rxpb
// "First Thermo" by frostbyte (thermometers) gas

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {0, 8, 0, 0, 0, 0, 0, 0, 0},
        {5, 0, 0, 0, 0, 4, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 4, 0, 0, 0, 0, 0, 0, 5},
        {0, 0, 0, 9, 0, 0, 0, 0, 0},
        {0, 0, 7, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 2, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 3, 0},
        {6, 0, 0, 0, 0, 0, 0, 0, 0}};
    vector<vector<pair<int, int>>> thermometerData = {
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2}}, // Thermometer 1 strictly increasing from (0,0) to (2,2)
        {{1, 7}, {1, 6}, {1, 5}, {2, 5}, {3, 5}, {3, 4}},
        {{5, 8}, {4, 8}, {4, 7}, {4, 6}, {3, 6}},
        {{7, 1}, {6, 1}, {6, 2}, {5, 2}, {5, 3}},
        {{8, 8}, {7, 8}, {7, 7}, {6, 7}, {6, 6}, {6, 5}}};

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
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
}