// https://sudokupad.app/gq9l9gbu2p
// "Fever Bends" by FullDeck and Missing a Few Cards
// Kropki Pairs // Thermometers
// Takes about 51 seconds on my machine
// 44 seconds with newer thermometers

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/kropkiModule.h"
#include "../modules/thermometerModule.h"

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

    vector<tuple<int, int, int, int, char>> kropkiData = {
        {1, 1, 2, 1, 'B'},
        {2, 3, 3, 3, 'B'},
        {1, 6, 1, 7, 'B'},
        {3, 5, 3, 6, 'B'},
        {4, 2, 4, 3, 'W'},
        {5, 2, 5, 3, 'B'},
        {5, 5, 6, 5, 'B'},
        {6, 6, 6, 7, 'B'},
        {6, 2, 7, 2, 'B'},
        {7, 0, 8, 0, 'B'}};

    vector<vector<pair<int, int>>> thermometersData = {
        {{2,0},{1,0},{1,1},{0,1},{0,2}},
        {{1,3},{1,2},{2,2},{2,1},{3,1}},
        {{0,6},{0,7},{1,7},{1,8},{2,8}},
        {{3,7},{2,7},{2,6},{1,6},{1,5}},
        {{5,1},{6,1},{6,2},{7,2},{7,3}},
        {{8,2},{8,1},{7,1},{7,0},{6,0}},
        {{7,5},{7,6},{6,6},{6,7},{5,7}},
        {{6,8},{7,8},{7,7},{8,7},{8,6}}
    };

    // Define constraints
    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add kropki constraint
    Constraint kropkiConstraint;
    kropkiConstraint.check = isKropkiSafe;
    kropkiConstraint.data = &kropkiData;
    constraints.push_back(kropkiConstraint);

    // Add thermometer constraint
    Constraint thermometerConstraint;
    thermometerConstraint.check = isThermometersSafe_StrictlyIncreasing;
    thermometerConstraint.data = &thermometersData;
    constraints.push_back(thermometerConstraint);

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