// https://sudokupad.app/hl1h0btfae
// "Quadriplex" by FullDeck and Missing a Few Cards
// Quadruples

//The density in the bottom right makes me think a solvemodularreverse would be more fitting
// Forward takes 585 seconds
// Reverse takes 501 seconds
// I guess I could have been correct, or it could have also been a coincidence because the TL=3 and BR=2 so BR had less iteration anyways
// I'll keep this as reverse just because it's somewhat faster

#include <iostream>
#include "../solvemodularreverse.h"
#include "../modules/quadruplesModule.h"

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

    std::vector<std::pair<std::vector<std::pair<int, int>>, std::vector<int>>> quadruplesData = {
        {{{0,0},{0,1},{1,0},{1,1}},{3,4,5}},
        {{{0,2},{0,3},{1,2},{1,3}},{3}},
        {{{0,7},{0,8},{1,7},{1,8}},{2,6}},
        {{{1,4},{1,5},{2,4},{2,5}},{2,7}},
        {{{2,2},{2,3},{3,2},{3,3}},{1,1,2,2}},
        {{{2,5},{2,6},{3,5},{3,6}},{3,3,4,4}},
        {{{3,1},{3,2},{4,1},{4,2}},{5,9}},
        {{{4,6},{4,7},{5,6},{5,7}},{1,5}},
        {{{5,2},{5,3},{6,2},{6,3}},{7,7,8,8}},
        {{{5,5},{5,6},{6,5},{6,6}},{5,5,6,6}},
        {{{6,3},{6,4},{7,3},{7,4}},{1,8}},
        {{{6,6},{6,7},{7,6},{7,7}},{7,9}},
        {{{7,0},{7,1},{8,0},{8,1}},{1,4}},
        {{{7,5},{7,6},{8,5},{8,6}},{8}},
        {{{7,7},{7,8},{8,7},{8,8}},{2,6,3}}
        };

    // Define the constraints
    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add Quadruples constraint
    Constraint quadruplesConstraint;
    quadruplesConstraint.check = isQuadruplesSafe;
    quadruplesConstraint.data = &quadruplesData;
    constraints.push_back(quadruplesConstraint);

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