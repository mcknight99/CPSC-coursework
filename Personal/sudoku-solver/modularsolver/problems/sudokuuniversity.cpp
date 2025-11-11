// https://www.twitch.tv/videos/2613236059 timestamp 21:38:31
//  contains killer cages all summing to 15 and some kropki black and white dots
// took 347 seconds (5 minutes and 47 seconds) to solve on my machine

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/cageModule.h"
#include "../modules/kropkiModule.h"

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

    // B between 0,0 and 0,1
    // W between 0,4 and 1,4
    // B between 0,8 and 1,8
    // B between 1,0 and 2,0
    // W between 1,8 and 2,8
    // B between 2,3 and 2,4
    // B between 2,6 and 3,6
    // B between 4,4 and 4,5
    // B between 5,2 and 6,2
    // W between 5,5 and 6,5
    // W between 6,1 and 6,2
    // B between 6,5 and 6,6
    // B between 7,0 and 8,0
    // W between 8,3 and 8,4
    // B between 8,7 and 8,8
    vector<tuple<int, int, int, int, char>> kropkiData = {
        {0, 0, 0, 1, 'B'},
        {0, 4, 1, 4, 'W'},
        {0, 8, 1, 8, 'B'},
        {1, 0, 2, 0, 'B'},
        {1, 8, 2, 8, 'W'},
        {2, 3, 2, 4, 'B'},
        {2, 6, 3, 6, 'B'},
        {4, 4, 4, 5, 'B'},
        {5, 2, 6, 2, 'B'},
        {5, 5, 6, 5, 'W'},
        {6, 1, 6, 2, 'W'},
        {6, 5, 6, 6, 'B'},
        {7, 0, 8, 0, 'B'},
        {8, 3, 8, 4, 'W'},
        {8, 7, 8, 8, 'B'}};

    // all cages sum to 15
    // 0,0 0,1 1,0
    // 0,4 1,4 2,4
    // 0,7 0,8 1,8
    // 2,2 2,3 4,2
    // 2,5 2,6 3,6
    // 4,0 4,1 4,2
    // 4,6 4,7 4,8
    // 5,2 6,2 6,3
    // 5,6 6,6 6,5
    // 6,4 7,4 8,4
    // 7,0 8,0 8,1
    // 7,8 8,8 8,7
    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{0, 0}, {0, 1}, {1, 0}}, 15},
        {{{0, 4}, {1, 4}, {2, 4}}, 15},
        {{{0, 7}, {0, 8}, {1, 8}}, 15},
        {{{2, 2}, {2, 3}, {3, 2}}, 15},
        {{{2, 5}, {2, 6}, {3, 6}}, 15},
        {{{4, 0}, {4, 1}, {4, 2}}, 15},
        {{{4, 6}, {4, 7}, {4, 8}}, 15},
        {{{5, 2}, {6, 2}, {6, 3}}, 15},
        {{{5, 6}, {6, 6}, {6, 5}}, 15},
        {{{6, 4}, {7, 4}, {8, 4}}, 15},
        {{{7, 0}, {8, 0}, {8, 1}}, 15},
        {{{7, 8}, {8, 8}, {8, 7}}, 15}};

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

    // Add kropki constraint
    Constraint kropkiConstraint;
    kropkiConstraint.check = isKropkiSafe;
    kropkiConstraint.data = &kropkiData;
    constraints.push_back(kropkiConstraint);

    // Solve the Sudoku puzzle
    if (solveModularSudoku(board, constraints))
    {
        cout << "Solution:\n";
        printBoard(board);
    }
    else
    {
        cout << "No solution found.\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken: " << duration.count() << " seconds" << endl;
}