// https://sudokupad.app/h2huu2ssqk
// "Tetris King Killer" by Kwame, Falconi, and Adam
// Antiking, Cages; indigestion
// Genuinely this one may take so much longer because of the start in the upper left corner
// If the solver started in the lower right corner, it might be(?) much faster
// Yippee I was correct, it took significantly shorter time because it didn't have to keep retrying the unknown upper area
// New runtime: 117 seconds

#include <iostream>
#include "../solvemodularreverse.h"
#include "../modules/antikingModule.h"
#include "../modules/cageModule.h"

#include <chrono>

using namespace std;

int main()
{

    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 7, 0, 0, 0, 0, 3},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 3, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 6, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 9, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 9}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{2, 0}, {3, 0}, {4, 0}, {5, 0}}, 22},
        {{{6, 0}, {7, 0}, {8, 0}, {8, 1}}, 10},
        {{{3, 1}, {3, 2}, {4, 1}, {4, 2}}, 20},
        {{{5, 1}, {5, 2}, {5, 3}, {6, 1}}, 18},
        {{{7, 1}, {7, 2}, {8, 2}, {8, 3}}, 19},
        {{{6, 2}, {6, 3}, {6, 4}, {6, 5}}, 25},
        {{{7, 3}, {7, 4}, {7, 5}, {8, 4}}, 23},
        {{{3, 4}, {4, 3}, {4, 4}, {5, 4}}, 18},
        {{{2, 6}, {2, 7}, {3, 5}, {3, 6}}, 21},
        {{{3, 7}, {3, 8}, {4, 8}, {5, 8}}, 27},
        {{{4, 5}, {4, 6}, {5, 5}, {5, 6}}, 18},
        {{{4, 7}, {5, 7}, {6, 7}, {7, 7}}, 10},
        {{{8, 5}, {8, 6}, {7, 6}, {6, 6}}, 16},
        {{{6, 8}, {7, 8}, {8, 8}, {8, 7}}, 27}};

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

    // Add antiking constraint
    Constraint antikingConstraint;
    antikingConstraint.check = isAntikingSafe;
    antikingConstraint.data = nullptr;
    constraints.push_back(antikingConstraint);

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