// https://sudokupad.app/bh4f01dzrn  (cages + thermometersSI) indigestion
// "Hashtag Thermometers" by FullDeck and Missing a Few Cards
// Cages, Thermometers; Indigestion
// This takes 322 seconds to run on my machine, so be patient.

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
        {{{0, 0}, {1, 0}, {0, 1}}, 20},
        {{{0, 5}, {1, 5}}, 6},
        {{{0, 7}, {0, 8}, {1, 8}}, 20},
        {{{2, 2}, {3, 2}, {2, 3}}, 14},
        {{{2, 4}, {3, 4}}, 9},
        {{{2, 5}, {2, 6}, {3, 6}}, 22},
        {{{4, 2}, {4, 3}}, 9},
        {{{4, 5}, {4, 6}}, 10},
        {{{5, 2}, {6, 2}, {6, 3}}, 22},
        {{{5, 4}, {6, 4}}, 10},
        {{{5, 6}, {6, 6}, {6, 5}}, 13},
        {{{7, 0}, {8, 0}, {8, 1}}, 15},
        {{{7, 3}, {8, 3}}, 12},
        {{{7, 8}, {8, 8}, {8, 7}}, 18}};

    vector<vector<pair<int,int>>> thermometerData = {
        {{2,0},{2,1},{2,2},{1,2},{0,2}},
        {{2,3},{2,4},{2,5}},
        {{0,6},{1,6},{2,6},{2,7},{2,8}},
        {{3,2},{4,2},{5,2}},
        {{5,6},{4,6},{3,6}},
        {{6,5},{6,4},{6,3}},
        {{8,2},{7,2},{6,2},{6,1},{6,0}},
        {{6,8},{6,7},{6,6},{7,6},{8,6}}
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

    // Add cages constraint
    Constraint cagesConstraint;
    cagesConstraint.check = isCagesSafe;
    // cagesConstraint.data = nullptr; // Replace with actual cages data
    cagesConstraint.data = &cagesData;
    constraints.push_back(cagesConstraint);

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
    cout<<"Time taken: "<<duration.count()<<" seconds"<<endl;
}