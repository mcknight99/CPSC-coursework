// I wanted to reapproach the sudoku solver by doing a logical approach, then once it reaches a point where it can't logically solve anymore, switch to a recursive backtracking approach
// I want to do this because of my work in progress Hashi solver, where I am taking the approach of logical -> recursive backtracking.
// Basically, solve as far as we can with logical methods until it requires guessing. Once it requires guesswork, switch to a recursive backtracking approach and reapply the logical methods until either:
// a. the puzzle is solved
// or b. backtrack to a previous puzzle state because the guess was incorrect

#include <iostream>
#include <vector>
// include sleep
#include <windows.h>

using namespace std;

// function prototypes
// return true if it is safe to place num in board[row][col]
bool isSafe(vector<vector<int>> &board, int row, int col, int num);
// return a 2d array of all possible numbers for each cell
vector<vector<int>> safeList(vector<vector<int>> &board);
// solve the sudoku puzzle using logical methods (if there is only one option from the safe list for a cell, place it in the cell)
bool solveLogically(vector<vector<int>> &board);
// solve the sudoku puzzle using recursive backtracking (if there are no single options, guess a safe number and see if it leads to a solution)
bool solveRecursively(vector<vector<int>> &board);
// print the sudoku board
void printBoard(vector<vector<int>> &board, const vector<pair<int, int>> &highlightedCells = {});
// is the sudoku puzzle solved (not a true checker, that happens in the solvers. this just detects if there are any remaining zeroes)
bool isSolved(vector<vector<int>> &board)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == 0)
                return false;
        }
    }
    return true;
}

int main()
{
    // example sudoku board
    vector<vector<int>> board =
        {
            {0,1,0,0,4,0,0,9,0},
            {9,0,0,6,0,3,0,0,1},
            {0,0,5,0,0,0,8,0,0},
            {0,3,0,4,0,5,0,2,0},
            {8,0,0,0,0,0,0,0,9},
            {0,5,0,1,0,9,0,3,0},
            {0,0,3,0,0,0,4,0,0},
            {5,0,0,7,0,2,0,0,6},
            {0,8,0,0,3,0,0,1,0}};

    // solve the sudoku puzzle
    while (!isSolved(board))
    {
        system("cls"); // clear the console
        if (!solveLogically(board))
        {
            if (!solveRecursively(board))
            {
                cout << "No solution exists!\n";
                return 0;
            }
        }
        Sleep(1000); // delay for 1 second
    }

    system("cls"); // clear the console
    // print the solved sudoku board
    printBoard(board);
    return 0;
}

// return true if it is safe to place num in board[row][col]
bool isSafe(vector<vector<int>> &board, int row, int col, int num)
{
    // check if num is not in the given row
    for (int x = 0; x < 9; x++)
    {
        if (board[row][x] == num)
        {
            return false;
        }
    }

    // check if num is not in the given column
    for (int x = 0; x < 9; x++)
    {
        if (board[x][col] == num)
        {
            return false;
        }
    }

    // check if num is not in the given 3x3 box
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i + startRow][j + startCol] == num)
            {
                return false;
            }
        }
    }
    return true;
}

// return a 2d array of all possible numbers for each cell
vector<vector<int>> safeList(vector<vector<int>> &board)
{
    vector<vector<int>> safeList(9, vector<int>(9, 0));
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == 0)
            {
                int count = 0;
                for (int num = 1; num <= 9; num++)
                {
                    if (isSafe(board, i, j, num))
                    {
                        count++;
                    }
                }
                safeList[i][j] = count;
            }
        }
    }
    return safeList;
}

// solve the sudoku puzzle using logical methods (if there is only one option from the safe list for a cell, place it in the cell)
bool solveLogically(vector<vector<int>> &board)
{
    vector<vector<int>> sl;
    sl = safeList(board);
    bool madeProgress = false;
    vector<pair<int, int>> highlightedCells;

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == 0 && sl[i][j] == 1)
            {
                for (int num = 1; num <= 9; num++)
                {
                    if (isSafe(board, i, j, num))
                    {
                        board[i][j] = num;
                        madeProgress = true;
                        std::cout << "Placed " << num << " in cell (" << i << "," << j << ")\n";
                        highlightedCells.push_back({i, j});
                        break;
                    }
                }
            }
        }
    }

    // if(!madeProgress) {
    //     solveRecursively(board);
    // }

    printBoard(board, highlightedCells);

    return madeProgress;
}

// solve the sudoku puzzle using recursive backtracking (if there are no single options, guess a safe number and see if it leads to a solution)
bool solveRecursively(vector<vector<int>> &board)
{

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == 0)
            {
                for (int num = 1; num <= 9; num++)
                {
                    if (isSafe(board, i, j, num))
                    {
                        board[i][j] = num;
                        if (solveRecursively(board))
                        {
                            return true;
                        }
                        board[i][j] = 0; // backtrack
                    }
                }
                return false; // no number can be placed in this cell
            }
        }
    }
    return true; // solved
}

// print the sudoku board
// highlights the cells that were filled in the last step using the highlightedCells parameter
void printBoard(vector<vector<int>> &board, const vector<pair<int, int>> &highlightedCells)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            bool highlighted = false;
            for (const auto &highlightedCell : highlightedCells)
            {
                if (highlightedCell.first == i && highlightedCell.second == j)
                {
                    highlighted = true;
                    cout << "\033[1;31m" << board[i][j] << "\033[0m "; // highlight in red
                }
            }
            if (!highlighted)
            {
                cout << board[i][j] << " ";
            }
        }
        cout << endl;
    }
}