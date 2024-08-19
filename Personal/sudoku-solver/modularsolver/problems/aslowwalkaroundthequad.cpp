// https://sudokupad.app/q5spa9ydxu
// "A Slow Walk Around the Quad" by FullDeck and Missing a Few Cards
// Thermometers (Non Decreasing) // Quadruples
// Took 4 mins 51 sec on my machine

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/thermometerModule.h"
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
        {{{0, 2}, {0, 3}, {1, 2}, {1, 3}}, {7, 7}},
        {{{2, 0}, {2, 1}, {3, 0}, {3, 1}}, {9, 9}},
        {{{2, 2}, {2, 3}, {3, 2}, {3, 3}}, {1, 1}},
        {{{2, 4}, {2, 5}, {3, 4}, {3, 5}}, {4, 4}},
        {{{2, 5}, {2, 6}, {3, 5}, {3, 6}}, {6, 6}},
        {{{2, 7}, {2, 8}, {3, 7}, {3, 8}}, {7, 7}},
        {{{5, 0}, {5, 1}, {6, 0}, {6, 1}}, {1, 2, 2, 7}},
        {{{5, 2}, {5, 3}, {6, 2}, {6, 3}}, {6, 6}},
        {{{5, 3}, {5, 4}, {6, 3}, {6, 4}}, {3, 3}},
        {{{5, 6}, {5, 7}, {6, 6}, {6, 7}}, {4, 4}},
        {{{7, 5}, {7, 6}, {8, 5}, {8, 6}}, {8, 8}}};

    vector<vector<pair<int, int>>> thermometerData = {
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {0, 3}},
        {{2, 2}, {3, 3}, {4, 2}, {5, 3}, {6, 2}},
        {{0, 6}, {1, 5}, {2, 5}, {3, 4}},
        {{6, 6}, {5, 5}, {4, 6}, {3, 5}, {2, 6}},
        {{8, 2}, {7, 3}, {6, 3}, {5, 4}},
        {{8, 8}, {7, 8}, {7, 7}, {7, 6}, {7, 5}, {8, 5}}};

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

    // Add Thermometer constraint
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