// https://app.crackingthecryptic.com/9x6y1zswz3
// "Nice Hot Cuppa" by FullDeck and Missing a Few Cards
// Palindromes // Quadruples
// Took 14 min 46 sec to run on my machine, so be patient
// Up to 17.5 with new thermo, aiming to fix this imbalance with newer thermo to have all decrease
// 17.37 with newer thermo, so it's a bit better

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/quadruplesModule.h"
#include "../modules/palindromeModule.h"

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
        {{{0, 4}, {0, 5}, {1, 4}, {1, 5}}, {4, 5, 8, 9}},
        {{{1, 6}, {1, 7}, {2, 6}, {2, 7}}, {1, 3, 7, 9}},
        {{{2, 2}, {2, 3}, {3, 2}, {3, 3}}, {3, 4, 6, 8}},
        {{{3, 6}, {3, 7}, {4, 6}, {4, 7}}, {1, 2, 5, 8}},
        {{{4, 4}, {4, 5}, {5, 4}, {5, 5}}, {3, 5, 7, 8}},
        {{{5, 5}, {5, 6}, {6, 5}, {6, 6}}, {3, 4, 5, 6}},
        {{{5, 0}, {5, 1}, {6, 0}, {6, 1}}, {1, 2, 3, 4}}};

    std::vector<std::vector<std::pair<int, int>>> palindromeData = {
        {{0, 2}, {1, 1}, {2, 1}, {3, 2}, {4, 2}, {5, 1}},
        {{0, 4}, {1, 3}, {2, 3}, {3, 4}, {4, 4}, {5, 3}},
        {{0, 6}, {1, 5}, {2, 5}, {3, 6}, {4, 6}, {5, 5}},
        {{6, 6}, {5, 6}, {5, 7}, {5, 8}, {6, 8}, {7, 7}, {7, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2}, {7, 1}, {6, 0}, {5, 0}}};

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

    // Add Palindromes constraint
    Constraint palindromeConstraint;
    palindromeConstraint.check = isPalindromeSafe;
    palindromeConstraint.data = &palindromeData;
    constraints.push_back(palindromeConstraint);

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