//https://sudokupad.app/fbfchhoqe2
// "Whoa, Oh Oh" by FullDeck and Missing a Few Cards
// Cages, Slow Thermos; heartburn
// Takes 87 seconds to run on my machine, so give it a minute (and 27 seconds)

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/cageModule.h"
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

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{1,2},{1,3}},5},
        {{{1,5},{1,6}},11},
        {{{2,0},{3,0}},9},
        {{{2,8},{3,8}},11},
        {{{5,0},{6,0}},11},
        {{{5,8},{6,8}},8},
        {{{7,2},{7,3}},9},
        {{{7,5},{7,6}},11}
    };

    vector<vector<pair<int,int>>> thermometerData = {
        {{2,0},{1,0},{0,0},{0,1},{0,2}},
        {{0,6},{0,7},{0,8},{1,8},{2,8}},
        {{8,2},{8,1},{8,0},{7,0},{6,0}},
        {{6,8},{7,8},{8,8},{8,7},{8,6}},
        {{2,2},{1,3},{2,4},{3,5},{2,6},{3,7},{4,8},{5,8},{6,7},{5,6},{4,5}},
        {{6,6},{7,5},{6,4},{5,3},{6,2},{5,1},{4,0},{3,0},{2,1},{3,2},{4,3}}
    };

    // Define constraints
    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add cages constraint
    Constraint cagesConstraint;
    cagesConstraint.check = isCagesSafe;
    cagesConstraint.data = &cagesData;
    constraints.push_back(cagesConstraint);

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
        cout << "No solution exists!\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken: " << duration.count() << " seconds" << endl;

}