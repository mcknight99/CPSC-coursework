#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

static string ANSI_RESET = "\u001B[0m";
static string ANSI_BLACK = "\u001B[30m";
static string ANSI_RED = "\u001B[31m";
static string ANSI_GREEN = "\u001B[32m";
static string ANSI_YELLOW = "\u001B[33m";
static string ANSI_BLUE = "\u001B[34m";
static string ANSI_PURPLE = "\u001B[35m";
static string ANSI_CYAN = "\u001B[36m";
static string ANSI_WHITE = "\u001B[37m";

// I know global variables is not great practice, but I'm using this next boolean for ease of use
// and not running the parameter down every recursive call to the solveSudoku method
bool verbose = false;

// Function to check if it's safe to place a number at a given position
bool isSafe(vector<vector<int>> &board, int row, int col, int num)
{
    // Check if the number is not already present in the current row, column, and 3x3 subgrid
    for (int i = 0; i < 9; ++i)
    {
        if (board[row][i] == num || board[i][col] == num || board[(row - (row % 3)) + (i / 3)][(col - (col % 3)) + (i % 3)] == num)
        {
            return false;
        }
    }
    return true;
}

// Function to print the Sudoku board
void printBoard(const vector<vector<int>> &board)
{
    for (int i = 0; i < 9; ++i)
    {
        if (i % 3 == 0 && i != 0)
        {
            cout << "---------------------\n";
        }
        for (int j = 0; j < 9; ++j)
        {
            if (j % 3 == 0 && j != 0)
            {
                cout << "| ";
            }
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to solve the Sudoku puzzle
// the changes to this method means that the original board remains unchanged after solving compared to the single solver
// verbose = true ('y') will print solutions as they are found, but will not replace the original board passed in
// verbose = false ('n') will not print solutions, but will replace the original board passed in with the solution (if one is found)
//         so the printing happens in the main, same as the single solver, solvesudoku.cpp
// in short, 'n' is the default behavior from solvesudoku.cpp, and 'y' is the new behavior to show all solutions found
bool solveSudoku(vector<vector<int>> &board)
{
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            // Find an empty cell
            if (board[row][col] == 0)
            {
                // Try placing numbers 1-9
                for (int num = 1; num <= 9; ++num)
                {
                    // Check if it's safe to place 'num' at the current position
                    if (isSafe(board, row, col, num))
                    {
                        // Place the number
                        board[row][col] = num;
                        // Recursively solve the rest of the puzzle
                        if (solveSudoku(board))
                        {
                            if (!verbose)
                            { //'n'
                                // return true to pass back the board state as the solved state up the recursion stack
                                return true;
                            }
                            // Print the solution
                            cout << ANSI_GREEN << "Solution:\n"
                                 << ANSI_RESET;
                            printBoard(board);
                            // Keep searching for more solutions
                        }
                        // Backtrack
                        board[row][col] = 0;
                    }
                }
                // If no number can be placed, return false - impossible case board
                // This is the effective base case that allows us to finally branch
                //      all the way back out of the solution recursion once there
                //      are no more solutions to be found anywhere
                return false;
            }
        }
    }
    // Return true if *a* solution was found recursively
    return true;
}

int main()
{
    cout << "Verbose output will show all solutions found, not just the first one.\n";
    cout << "Verbose output will take long to run as it has to check all valid cases.\n";
    cout << "Would you like to see verbose output? (y/n, default n): ";
    char response;
    cin >> response;
    if (response == 'y' || response == 'Y')
    {
        verbose = true;
    }

    vector<vector<vector<int>>> boardList = {
        {
            {4, 0, 0, 0, 3, 1, 0, 0, 9},
            {3, 2, 0, 0, 0, 5, 6, 0, 8},
            {0, 5, 1, 8, 0, 6, 2, 0, 0},
            {8, 0, 0, 5, 0, 0, 1, 9, 0},
            {5, 0, 2, 0, 8, 0, 0, 0, 0},
            {1, 6, 0, 3, 2, 9, 5, 0, 7},
            {0, 4, 9, 6, 0, 0, 0, 2, 0},
            {2, 1, 8, 0, 5, 0, 0, 0, 4},
            {6, 0, 5, 4, 0, 2, 8, 0, 1}}, // the one that started it all; my newspaper sudoku
        {
            {1, 2, 3, 0, 0, 7, 0, 0, 8},
            {4, 5, 6, 1, 0, 0, 0, 0, 0},
            {7, 8, 9, 0, 0, 0, 2, 0, 0},
            {0, 0, 0, 2, 3, 4, 5, 0, 0},
            {6, 0, 0, 9, 1, 5, 0, 0, 0},
            {0, 0, 0, 8, 7, 6, 0, 0, 0},
            {9, 0, 0, 0, 0, 0, 3, 2, 1},
            {0, 0, 0, 0, 0, 0, 6, 5, 4},
            {0, 4, 0, 3, 0, 0, 9, 8, 7}}, // my testing board for finding several solutions
        {
            {0, 8, 0, 0, 0, 0, 0, 0, 1},
            {0, 0, 7, 0, 0, 4, 0, 2, 0},
            {6, 0, 0, 3, 0, 0, 7, 0, 0},
            {0, 0, 2, 0, 0, 9, 0, 0, 0},
            {1, 0, 0, 0, 6, 0, 0, 0, 8},
            {0, 3, 0, 4, 0, 0, 0, 0, 0},
            {0, 0, 1, 7, 0, 0, 6, 0, 0},
            {0, 9, 0, 0, 0, 8, 0, 0, 5},
            {0, 0, 0, 0, 0, 0, 0, 4, 0}}, // "AI worm hole", one of the hardest sudoku boards with only one solution from Arto Inkala
        {
            {4, 0, 0, 0, 6, 0, 0, 7, 0},
            {0, 0, 0, 0, 0, 0, 6, 0, 0},
            {0, 3, 0, 0, 0, 2, 0, 0, 1},
            {7, 0, 0, 0, 0, 8, 5, 0, 0},
            {0, 1, 0, 4, 0, 0, 0, 0, 0},
            {0, 2, 0, 9, 5, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 7, 0, 5},
            {0, 0, 9, 1, 0, 0, 0, 3, 0},
            {0, 0, 3, 0, 4, 0, 0, 8, 0}}, // "AI broken brick", one of the hardest sudoku boards with only one solution from Arto Inkala
        {
            {1, 0, 0, 0, 0, 7, 0, 9, 0},
            {0, 3, 0, 0, 2, 0, 0, 0, 8},
            {0, 0, 9, 6, 0, 0, 5, 0, 0},
            {0, 0, 5, 3, 0, 0, 9, 0, 0},
            {0, 1, 0, 0, 8, 0, 0, 0, 2},
            {6, 0, 0, 0, 0, 4, 0, 0, 0},
            {3, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 4, 0, 0, 0, 0, 0, 0, 7},
            {0, 0, 7, 0, 0, 0, 3, 0, 0}}, // "AI Escargot", the hardest single solution sudoku from Arto Inkala
        {
            {8, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 3, 6, 0, 0, 0, 0, 0},
            {0, 7, 0, 0, 9, 0, 2, 0, 0},
            {0, 5, 0, 0, 0, 7, 0, 0, 0},
            {0, 0, 0, 0, 4, 5, 7, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 3, 0},
            {0, 0, 1, 0, 0, 0, 0, 6, 8},
            {0, 0, 8, 5, 0, 0, 0, 1, 0},
            {0, 9, 0, 0, 0, 0, 4, 0, 0}}, // supposedly the hardest single solution sudoku from Arto Inkala https://abcnews.go.com/blogs/headlines/2012/06/can-you-solve-the-hardest-ever-sudoku
                                          // this one actually does take longer than all the others to "solve" as it has the most branches to explore when solving
                                          // but if it returned as soon as it found the first solution, it would be equivalent time

    };

    for (vector<vector<int>> board : boardList)
    {
        auto start = std::chrono::high_resolution_clock::now();

        cout << ANSI_BLUE << "\nOriginal board:\n"
             << ANSI_RESET;
        printBoard(board);

        if (solveSudoku(board))
        {
            if (!verbose)
            { //'n'
                cout << ANSI_GREEN << "Solution:\n"
                     << ANSI_RESET;
                printBoard(board); // non verbose changes the original board, so it prints here
            }
            // verbose output already prints the solutions as they are found
        }
        else
        {
            // both verbose and non-verbose output will print this message when no solution is found
            cout << ANSI_RED << "Out of solutions!\n"
                 << ANSI_RESET;
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        cout << ANSI_YELLOW << "Time taken: " << duration.count() << " milliseconds\n\n"
             << ANSI_RESET;
    }

    return 0;
}
