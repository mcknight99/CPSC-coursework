// https://sudokupad.app/i2yg8yesd3
// "Tardigrades" by FullDeck and Missing a Few Cards
// Thermometers
// Takes about 15 minutes to run on my machine

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main()
{

    vector<vector<int>> board = {
        {0, 0, 0, 0, 0, 0, 0, 0, 5},
        {0, 6, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {9, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 4, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    // This is only my second problem using multi-ending thermometers
    // But my first one with newer implementation; I think with newer implementation doing
    // separate thermometers for each end rather than branching onto the last bit of the body
    // will speed up the solution. I'll try both and see which is faster.
    // Bulb to tip = 868 seconds
    // Branch to tip = 1000 seconds
    // So the hypothesis was correct, making individual bulb to tips is faster than branching to the tip

    // This is the bulb to tip multi ended thermometer
    vector<vector<pair<int, int>>> thermometerData_bulbToTip = {
        {{0, 2}, {0, 3}, {1, 3}},
        {{0, 2}, {0, 3}, {0, 4}, {1, 4}},
        {{0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 5}},
        {{0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 6}},
        {{3, 1}, {3, 2}, {4, 2}},
        {{3, 1}, {3, 2}, {3, 3}, {4, 3}},
        {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {4, 4}},
        {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {4, 5}},
        {{6, 4}, {6, 5}, {5, 5}},
        {{6, 4}, {6, 5}, {6, 6}, {5, 6}},
        {{6, 4}, {6, 5}, {6, 6}, {6, 7}, {5, 7}},
        {{6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {5, 8}},
        {{8, 0}, {7, 0}, {7, 1}}, // What's crazy is that copilot caught onto the pattern here and basically filled in the last two from me giving it the bulb and first tip
        {{8, 0}, {7, 0}, {6, 0}, {6, 1}},
        {{8, 0}, {7, 0}, {6, 0}, {5, 0}, {5, 1}},
        {{8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {4, 1}},
        {{8, 6}, {8, 5}, {7, 5}},
        {{8, 6}, {8, 5}, {8, 4}, {7, 4}},
        {{8, 6}, {8, 5}, {8, 4}, {8, 3}, {7, 3}},
        {{8, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2}, {7, 2}}};

    // This is the branch to tip multi ended thermometer
    vector<vector<pair<int, int>>> thermometerData_branchToTip = {
        {{0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 6}},
        {{0, 3}, {1, 3}},
        {{0, 4}, {1, 4}},
        {{0, 5}, {1, 5}},
        {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {4, 5}},
        {{3, 2}, {4, 2}},
        {{3, 3}, {4, 3}},
        {{3, 4}, {4, 4}},
        {{6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {5, 8}},
        {{6, 5}, {5, 5}},
        {{6, 6}, {5, 6}},
        {{6, 7}, {5, 7}},
        {{8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {4, 1}},
        {{7, 0}, {7, 1}},
        {{6, 0}, {6, 1}},
        {{5, 0}, {5, 1}},
        {{8, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2}, {7, 2}}, // Copilot was trying its best but didn't get the autofill perfect until here
        {{8, 5}, {7, 5}},
        {{8, 4}, {7, 4}},
        {{8, 3}, {7, 3}}};

    // Bulb to tip constraints
    vector<Constraint> constraints_bulbToTip;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints_bulbToTip.push_back(basicConstraint);

    // Add thermometer constraint
    Constraint thermometerConstraint_bulbToTip;
    thermometerConstraint_bulbToTip.check = isThermometersSafe_StrictlyIncreasing;
    thermometerConstraint_bulbToTip.data = &thermometerData_bulbToTip;
    constraints_bulbToTip.push_back(thermometerConstraint_bulbToTip);

    // Branch to tip constraints
    vector<Constraint> constraints_branchToTip;

    // Add Sudoku basic constraint
    constraints_branchToTip.push_back(basicConstraint);

    // Add thermometer constraint
    Constraint thermometerConstraint_branchToTip;
    thermometerConstraint_branchToTip.check = isThermometersSafe_StrictlyIncreasing;
    thermometerConstraint_branchToTip.data = &thermometerData_branchToTip;
    constraints_branchToTip.push_back(thermometerConstraint_branchToTip);

    auto start_bulb = chrono::high_resolution_clock::now();

    // 868 seconds
    //  Solve the bulb to tip Sudoku puzzle
    
    if (solveModularSudoku(board, constraints_bulbToTip))
    {
        cout << "Solution:\n";
        printBoard(board);
    }
    else
    {
        cout << "No solution exists\n";
    }

    auto stop_bulb = chrono::high_resolution_clock::now();
    auto duration_bulb = chrono::duration_cast<chrono::seconds>(stop_bulb - start_bulb);
    cout << "Time taken (bulb to tip): " << duration_bulb.count() << " seconds" << endl;
    

    // Bang-on 1000 seconds
    /*
        board = {
            {0, 0, 0, 0, 0, 0, 0, 0, 5},
            {0, 6, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {9, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 4, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0}};
        auto start_branch = chrono::high_resolution_clock::now();

        // Solve the Sudoku puzzle

        if (solveModularSudoku(board, constraints_branchToTip))
        {
            cout << "Solution:\n";
            printBoard(board);
        }
        else
        {
            cout << "No solution exists\n";
        }

        auto stop_branch = chrono::high_resolution_clock::now();
        auto duration_branch = chrono::duration_cast<chrono::seconds>(stop_branch - start_branch);
        cout << "Time taken (branch to tip): " << duration_branch.count() << " seconds" << endl;
        */
}