#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {5, 0, 0, 0, 8, 6, 0, 0, 1},
        {0, 0, 2, 7, 0, 1, 6, 0, 0},
        {0, 7, 1, 0, 0, 0, 2, 5, 0},

        {9, 1, 0, 0, 2, 0, 0, 7, 0},
        {3, 0, 0, 1, 4, 5, 0, 0, 6},
        {0, 6, 0, 0, 9, 0, 0, 2, 4},

        {0, 5, 3, 0, 0, 0, 4, 6, 0},
        {0, 0, 8, 9, 0, 3, 5, 0, 0},
        {2, 0, 0, 5, 1, 0, 0, 0, 7}};

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    // Solve the Sudoku puzzle
    if (solveModularSudoku(board, constraints))
    {
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Solution:\n";
        printBoard(board);
    }
    else
    {
        cout << "No solution exists\n";
    }

    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;
}