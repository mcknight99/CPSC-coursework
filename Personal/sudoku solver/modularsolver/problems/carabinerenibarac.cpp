// https://sudokupad.app/zennhfif3c
// "CarabinerenibaraC" by FullDeck and Missing a Few Cards
// Killer Cages // Palindromes
// With all fives, this takes 13.5 mins to solve. Would not recommend without the fives, it ran for over two hours before I stopped it.

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/cageModule.h"
#include "../modules/palindromeModule.h"

#include <chrono>

using namespace std;

int main()
{

    auto start = chrono::high_resolution_clock::now();

    // The sudoku is only with the 4 and 6, with the rest as zeroes. But this takes way too long to solve with my current implementation
    // So I added all of the fives below (I didn't want to put any numbers in the cages or palindromes to let the constraints work themselves out and using the same number everywhere to give it the least amount of information)
    /*vector<vector<int>> board = {
        {0, 5, 0, 0, 0, 0, 0, 0, 6},
        {0, 0, 0, 0, 5, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 5, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 5, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 5}};*/
    vector<vector<int>> board = {
        {0, 0, 0, 0, 0, 0, 0, 0, 6},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0}};

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{1, 5}, {2, 5}}, 15},
        {{{2, 6}, {3, 6}}, 11},
        {{{3, 7}, {4, 7}}, 6},
        {{{3, 8}, {4, 8}, {5, 8}}, 16},
        {{{5, 5}, {5, 6}, {5, 7}}, 16},
        {{{3, 1}, {3, 2}, {3, 3}}, 8},
        {{{4, 1}, {5, 1}}, 13},
        {{{5, 2}, {6, 2}}, 6},
        {{{6, 3}, {7, 3}}, 5}};

    std::vector<std::vector<std::pair<int, int>>> palindromeData = {
        {{3, 0}, {2, 0}, {1, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 5}},
        {{0, 6}, {1, 6}, {2, 6}, {3, 7}, {3, 8}},
        {{3, 3}, {2, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 7}},
        {{7, 3}, {8, 4}, {8, 5}, {8, 6}, {7, 7}, {6, 8}, {5, 8}},
        {{3, 1}, {4, 1}, {5, 2}, {6, 3}, {6, 4}, {5, 5}},
        {{5, 0}, {5, 1}, {6, 2}, {7, 2}, {8, 2}}};

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

    // Add palindromes constraint
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