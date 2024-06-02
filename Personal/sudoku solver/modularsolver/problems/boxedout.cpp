// https://sudokupad.app/agtakyemot
// "Boxed Out" by Preston
// Thermometers

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {0, 0, 0, 0, 0, 0, 7, 1, 2},
        {0, 0, 0, 0, 7, 0, 0, 0, 0},
        {0, 7, 0, 0, 8, 9, 0, 0, 0},
        {0, 2, 0, 0, 0, 0, 0, 5, 9},
        {0, 0, 0, 0, 0, 0, 0, 3, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 8, 2, 0, 0, 0, 0},
        {6, 1, 0, 0, 9, 0, 0, 8, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    vector<vector<pair<int, int>>> thermometerData = {
        {{2,0},{1,0},{1,1},{1,2},{2,2}},
        {{3,0},{4,0},{4,1},{4,2},{3,2}},
        {{1,3},{0,3},{0,4},{0,5},{1,5}},
        {{2,3},{3,3},{3,4},{3,5},{2,5}},
        {{3,6},{2,6},{2,7},{2,8},{3,8}},
        {{4,6},{5,6},{5,7},{5,8},{4,8}},
        {{5,5},{5,4},{5,3},{6,3}},
        {{6,5},{7,5},{8,5},{8,4},{8,3},{7,3}}        
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