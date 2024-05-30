// https://sudokupad.app/i8sojnbdyr
// "Killer Pinwheel" by Adam
// Killer Cages // Thermometers
// 7 seconds on my machine
// 3.5 seconds with newer thermos

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/cageModule.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main()
{

    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {0, 0, 9, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 5, 0, 0, 9},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 3, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {7, 0, 0, 0, 0, 0, 0, 5, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 8, 0, 0, 0, 0, 0}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{1, 4}, {1, 5}, {2, 4}}, 16},
        {{{1, 6}, {1, 7}, {2, 6}}, 12},
        {{{4, 3}, {4, 4}, {4, 5}}, 19},
        {{{3, 6}, {3, 7}, {4, 6}}, 19},
        {{{5, 4}, {6, 3}, {6, 4}}, 18},
        {{{6, 1}, {6, 2}, {7, 1}}, 12},
        {{{6, 6}, {7, 6}, {7, 7}}, 12}};

    vector<vector<pair<int, int>>> thermometerData = {
        {{0, 0}, {1, 1}, {2, 0}, {3, 1}, {4, 0}, {5, 1}},
        {{0, 8}, {1, 7}, {0, 6}, {1, 5}, {0, 4}, {1, 3}},
        {{8, 8}, {7, 7}, {6, 8}, {5, 7}, {4, 8}, {3, 7}},
        {{8, 0}, {7, 1}, {8, 2}, {7, 3}, {8, 4}, {7, 5}},
        {{3, 5}, {3, 4}, {3, 3}, {4, 3}},
        {{5, 3}, {5, 4}, {5, 5}, {4, 5}}};

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add Killer Cages constraint
    Constraint cageConstraint;
    cageConstraint.check = isCagesSafe;
    cageConstraint.data = &cagesData;
    constraints.push_back(cageConstraint);

    // Add Thermometer constraint
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
        cout << "No solution exists!\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
}