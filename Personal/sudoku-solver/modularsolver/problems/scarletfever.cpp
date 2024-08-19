// https://sudokupad.app/y4lnjzh1g0
// "Scarlet Fever" by FullDeck and Missing a Few Cards
// Kropki Pairs // Thermometers
// Takes about 80 seconds to run on my machine
// 49 seconds with newer thermos

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
        {0, 2, 1, 2, 'B'},
        {2, 4, 3, 4, 'B'},
        {2, 7, 2, 8, 'B'},
        {2, 7, 3, 7, 'W'},
        {3, 6, 4, 6, 'B'},
        {4, 2, 4, 3, 'B'},
        {5, 0, 6, 0, 'B'},
        {6, 7, 6, 8, 'B'},
        {8, 5, 8, 6, 'B'}};

    vector<vector<pair<int, int>>> thermometersData = {
        {{2, 0}, {1, 0}, {0, 0}, {0, 1}, {0, 2}},
        {{2, 8}, {1, 8}, {0, 8}, {0, 7}, {0, 6}},
        {{4, 2}, {3, 2}, {2, 2}, {2, 3}, {2, 4}},
        {{4, 6}, {5, 6}, {6, 6}, {6, 5}, {6, 4}},
        {{6, 0}, {7, 0}, {8, 0}, {8, 1}, {8, 2}},
        {{6, 8}, {7, 8}, {8, 8}, {8, 7}, {8, 6}}};

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