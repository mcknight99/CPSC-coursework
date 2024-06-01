// https://sudokupad.app/ealf9hvq0r
// "Beaker on a Slow Burner" by FullDeck and Missing a Few Cards
// Thermometers 

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main() {
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

    vector<vector<pair<int, int>>> thermometerData = {
        {{0,0},{1,0},{2,0},{3,1},{4,0},{5,1},{6,0}},
        {{8,1},{8,2}},
        {{6,4},{5,4},{4,4},{3,4},{2,4}},
        {{8,7},{8,6}},
        {{0,8},{1,8},{2,8},{3,7},{4,8},{5,7},{6,8}},
        {{1,6},{2,5},{3,6},{4,5},{5,6},{6,5},{7,6},{8,5},{8,4},{8,3},{7,2},{6,3},{5,2},{4,3},{3,2},{2,3},{1,2}}
    };

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add thermometer constraint
    Constraint thermometerConstraint;
    thermometerConstraint.check = isThermometersSafe_Nondecreasing;
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
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken: " << duration.count() << " seconds" << endl;


}