// https://sudokupad.app/9vlbeverf9
// "15 with 14 Differences" by FullDeck and Missing a Few Cards
// Kropki Pairs // Killer Cages

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/kropkiModule.h"
#include "../modules/cageModule.h"

#include <chrono>

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 2, 0, 0, 3, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 4, 0, 0, 5, 0, 0, 6, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 8, 0, 0, 9, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    vector<tuple<int, int, int, int, char>> kropkiData = {
        {0,1,1,1,'W'},
        {0,4,1,4,'W'},
        {0,6,1,6,'W'},
        {1,6,1,7,'W'},
        {1,2,1,3,'W'},
        {2,8,3,8,'W'},
        {3,2,3,3,'W'},
        {5,0,6,0,'W'},
        {5,5,5,6,'W'},
        {7,1,7,2,'W'},
        {7,2,8,2,'W'},
        {7,4,8,4,'W'},
        {7,5,7,6,'W'},
        {7,7,8,7,'W'}
        };

    vector<pair<vector<pair<int, int>>, int>> cagesData = {
        {{{0,0},{0,1},{1,0}},15},
        {{{0,7},{0,8},{1,8}},15},
        {{{2,1},{2,2},{2,3}},15},
        {{{2,4},{2,5},{3,5}},15},
        {{{1,3},{1,4},{1,5}},15},
        {{{3,1},{4,1},{5,1}},15},
        {{{3,3},{3,2},{4,2}},15},
        {{{5,2},{6,2},{7,2}},15},
        {{{7,0},{8,0},{8,1}},15},
        {{{5,3},{6,3},{6,4}},15},
        {{{7,3},{7,4},{7,5}},15},
        {{{4,6},{5,6},{5,5}},15},
        {{{1,6},{2,6},{3,6}},15},
        {{{3,7},{4,7},{5,7}},15},
        {{{6,5},{6,6},{6,7}},15},
        {{{7,8},{8,8},{8,7}},15}
        };

    // Define constraints
    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add kropki constraint
    Constraint kropkiConstraint;
    kropkiConstraint.check = isKropkiSafe;
    kropkiConstraint.data = &kropkiData;
    constraints.push_back(kropkiConstraint);

    // Add cages constraint
    Constraint cagesConstraint;
    cagesConstraint.check = isCagesSafe;
    cagesConstraint.data = &cagesData;
    constraints.push_back(cagesConstraint);

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