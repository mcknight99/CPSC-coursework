#include <iostream>
#include "../solvemodularlogically.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {5,0,0, 0,8,6, 0,0,1},
        {0,0,2, 7,0,1, 6,0,0},
        {0,7,1, 0,0,0, 2,5,0},

        {9,1,0, 0,2,0, 0,7,0},
        {3,0,0, 1,4,5, 0,0,6},
        {0,6,0, 0,9,0, 0,2,4},

        {0,5,3, 0,0,0, 4,6,0},
        {0,0,8, 9,0,3, 5,0,0},
        {2,0,0, 5,1,0, 0,0,7}
    };

    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = getLogicalNums;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Solve the Sudoku puzzle
    vector<Step> steps = solveModularLogically(board, constraints);
    
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    if (steps.size() > 0)
    {
        cout << "Solution:\n";
        printBoard(board);
    }
    
    else
    {
        cout << "No solution found\n";
    }
    for(auto step : steps)
    {
        cout << "(" <<step.row << ", " << step.col << ") -> " << step.value << ", \t";
    }
    cout << endl;

    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;
}