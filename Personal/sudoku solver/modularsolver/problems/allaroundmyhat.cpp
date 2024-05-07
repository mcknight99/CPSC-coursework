// https://sudokupad.app/90rrvn1ygk
// "All Around My Hat" by FullDeck and Missing a Few Cards
// Takes about 80 seconds to solve on my machine, be patient if you run this

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/cageModule.h"

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
        {{{0,0},{1,0}},12}, // Cells (0,0) and (1,0) form a cage with sum 12
        {{{0,2},{1,2}},3},
        {{{0,3},{0,4}},11},
        {{{0,5},{1,5}},7},
        {{{0,6},{0,7}},12},
        {{{0,8},{1,8}},11},
        {{{2,0},{2,1}},7},
        {{{2,3},{2,4}},11},
        {{{2,6},{2,7}},3},
        {{{3,0},{4,0}},9},
        {{{3,2},{4,2}},9},
        {{{3,3},{3,4}},8},
        {{{3,5},{4,5}},13},
        {{{3,6},{4,6}},8},
        {{{3,8},{4,8}},17},
        {{{5,0},{5,1}},13},
        {{{5,3},{5,4}},17},
        {{{5,6},{5,7}},9},
        {{{6,2},{7,2}},15},
        {{{6,4},{7,4}},7},
        {{{6,5},{7,5}},10},
        {{{6,7},{7,7}},14},
        {{{6,8},{7,8}},5},
        {{{8,0},{8,1}},5},
        {{{8,3},{8,4}},15},
        {{{8,6},{8,7}},10}
    };

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

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