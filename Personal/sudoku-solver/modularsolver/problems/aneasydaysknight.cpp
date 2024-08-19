// https://app.crackingthecryptic.com/68izjadc6y an easy day's knight (antiknight)
// "An Easy Day's Knight" by FullDeck and Missing a Few Cards
// Antiknight; gas

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/antiknightModule.h"

#include <chrono>

using namespace std;

int main()
{

    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {1, 6, 0, 0, 2, 0, 0, 0, 3},
        {0, 0, 0, 0, 0, 0, 0, 0, 8},
        {0, 0, 9, 0, 6, 0, 7, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 2, 0, 5, 0, 8, 0, 6},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 0, 4, 0, 1, 0, 0},
        {2, 0, 0, 0, 0, 0, 0, 0, 0},
        {7, 0, 0, 0, 8, 0, 0, 4, 9}};

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add antiknight constraint
    Constraint antiknightConstraint;
    antiknightConstraint.check = isAntiknightSafe;
    antiknightConstraint.data = nullptr;
    constraints.push_back(antiknightConstraint);

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