// https://sudokupad.app/qd8sics5tx
// "Quadriplexing" by FullDeck and Missing a Few Cards
// Antiknight // Quadruples

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/antiknightModule.h"
#include "../modules/quadruplesModule.h"

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

    std::vector<std::pair<std::vector<std::pair<int, int>>, std::vector<int>>> quadruplesData = {
        {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}, {1, 2, 3, 8}},
        {{{1, 1}, {1, 2}, {2, 1}, {2, 2}}, {4, 5, 6}},
        {{{2, 2}, {2, 3}, {3, 2}, {3, 3}}, {2, 8}},
        {{{3, 1}, {4, 1}, {3, 2}, {4, 2}}, {1, 4}},
        {{{3, 3}, {3, 4}, {4, 3}, {4, 4}}, {2}},
        {{{4, 3}, {4, 4}, {5, 3}, {5, 4}}, {6}},
        {{{5, 2}, {5, 3}, {6, 2}, {6, 3}}, {1, 5}},
        {{{6, 1}, {6, 2}, {7, 1}, {7, 2}}, {6, 7, 8}},
        {{{7, 0}, {7, 1}, {8, 0}, {8, 1}}, {1, 4, 5, 9}},
        {{{0, 7}, {0, 8}, {1, 7}, {1, 8}}, {4, 7, 8, 9}},
        {{{1, 6}, {1, 7}, {2, 6}, {2, 7}}, {4, 5, 6}},
        {{{2, 5}, {2, 6}, {3, 5}, {3, 6}}, {7, 9}},
        {{{3, 4}, {3, 5}, {4, 4}, {4, 5}}, {3}},
        {{{4, 4}, {4, 5}, {5, 4}, {5, 5}}, {5}},
        {{{5, 5}, {5, 6}, {6, 5}, {6, 6}}, {2, 4}},
        {{{4, 7}, {4, 8}, {5, 7}, {5, 8}}, {2, 6}},
        {{{6, 6}, {6, 7}, {7, 6}, {7, 7}}, {4, 5, 6}},
        {{{7, 7}, {7, 8}, {8, 7}, {8, 8}}, {1, 2, 3, 7}}};

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

    // Add Antiknight constraint
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