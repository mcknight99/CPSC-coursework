// https://sudokupad.app/ek2cvdxj8h
//  "Cage Practice" by FullDeck and Missing a Few Cards
//  Cages; gas
// It took 920 seconds on my machine to find the solution
// I could configure the solver to print all solutions so I could find the one on the website,
// but this seems more unique, and I don't want to wait around for a second solution
//  actually i went ahead and did it anyways because it was easy, but now it takes forever to run because of all of the states

// I FOUND A SOLUTION THAT ISNT ACCEPTED BY THE WEBSITE
/*
Solution:
1 3 2 | 4 8 7 | 9 5 6
6 7 4 | 2 9 5 | 1 8 3
8 9 5 | 1 6 3 | 2 4 7
---------------------
2 4 6 | 8 3 1 | 7 9 5
9 8 7 | 6 5 4 | 3 2 1
5 1 3 | 9 7 2 | 4 6 8
---------------------
3 6 8 | 7 4 9 | 5 1 2
7 2 9 | 5 1 6 | 8 3 4
4 5 1 | 3 2 8 | 6 7 9

(THE REAL SOLUTION IS THE 8 AND 6 SWITCHED IN r8c6-r9c6 and r8c7-r9c7)
*/

#include <iostream>
#include "../solveseveralmodular.h"
#include "../modules/cageModule.h"

#include <chrono>

using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // Define the Sudoku board
    vector<vector<int>> board = {
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{0, 0}, {0, 1}, {0, 2}}, 6}, // Squares in the cage (0,0), (0,1), (0,2) must sum to 6
        {{{0, 3}, {1, 3}, {2, 3}}, 7},
        {{{0, 5}, {0, 6}, {1, 5}}, 21},
        {{{1, 1}, {2, 1}, {3, 1}}, 20},
        {{{1, 7}, {2, 7}}, 12},
        {{{2, 5}, {2, 6}}, 5},
        {{{3, 2}, {4, 2}}, 13},
        {{{3, 6}, {3, 7}}, 16},
        {{{4, 0}, {4, 1}}, 17},
        {{{4, 3}, {4, 4}, {4, 5}}, 15},
        {{{4, 6}, {5, 6}}, 7},
        {{{4, 7}, {4, 8}}, 3},
        {{{5, 1}, {5, 2}}, 4},
        {{{5, 7}, {6, 7}, {7, 7}}, 10},
        {{{6, 1}, {7, 1}}, 8},
        {{{6, 2}, {6, 3}}, 15},
        {{{6, 5}, {7, 5}, {8, 5}}, 23},
        {{{7, 3}, {8, 3}, {8, 2}}, 9},
        {{{8, 6}, {8, 7}, {8, 8}}, 24}};

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

    // Solve the Sudoku puzzle
    if (!solveModularSudoku(board, constraints))
    {
        cout<< "Out of solutions\n";
        //cout << "Solution:\n";
        //printBoard(board);
    }
    else
    {
        cout << "what HUH HOW\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken: " << duration.count() << " seconds" << endl;
}