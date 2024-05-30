#include <iostream>
#include <vector>

using namespace std;

// Function to check if it's safe to place a number at a given position
bool isSafe(vector<vector<int>>& board, int row, int col, int num) {
    // Check if the number is not already present in the current row, column, and 3x3 subgrid
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num || board[(row - (row % 3)) + (i / 3)][(col - (col % 3)) + (i % 3)] == num) {
            return false;
        }
    }
    return true;
}

// This solver is in no way optimized, as it tries all possible numbers at each empty cell rather than using more advanced techniques, 
// such as taking note of each valid number for each cell and then trying to solve the puzzle by prioritizing cells with fewer valid numbers.
// However, doing so would make the code more complex and harder to understand or modify. 
// Additionally, the "advanced" example I use of noting valid numbers may also have the downside of longer runtimes, as testing and failing in a cell effectively does this job already, 
// and may be faster than constantly checking all valid numbers for each cell. 
// (Perhaps an update by 3x3, row, and col for each cell with that method that I'm describing would be a solution, but I'm already happy with this solver. Feel free to try it out!)

// Function to solve the Sudoku puzzle
bool solveSudoku(vector<vector<int>>& board) {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            // Find empty cell
            if (board[row][col] == 0) {
                // Try placing numbers 1-9
                for (int num = 1; num <= 9; ++num) {
                    // Check if it's safe to place 'num' at the current position
                    if (isSafe(board, row, col, num)) {
                        // Place the number
                        board[row][col] = num;
                        cout<<"trying to place "<<num<<" at "<<row<<","<<col<<"\n";

                        // Recursively solve the rest of the puzzle
                        if (solveSudoku(board)) {
                            cout<<"placed "<<num<<" at "<<row<<","<<col<<"\n";
                            return true;
                        }

                        // If placing 'num' doesn't lead to a solution, backtrack
                        cout<<"backtracking from "<<num<<" at "<<row<<","<<col<<"\n";
                        board[row][col] = 0;
                    }
                }
                // If no number can be placed, return false
                return false;
            }
        }
    }
    // If all cells are filled, return true
    return true;
}

// Function to print the Sudoku board
void printBoard(const vector<vector<int>>& board) {
    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0 && i != 0) {
            cout << "---------------------\n";
        }
        for (int j = 0; j < 9; ++j) {
            if (j % 3 == 0 && j != 0) {
                cout << "| ";
            }
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}


int main() {
    vector<vector<int>> board = {
        {4,0,0,0,3,1,0,0,9},
        {3,2,0,0,0,5,6,1,8},
        {9,5,1,8,0,6,2,0,0},
        {8,0,0,5,0,0,1,9,0},
        {5,0,2,0,8,0,0,0,0},
        {1,6,0,3,2,9,5,0,7},
        {0,4,9,6,0,0,0,2,0},
        {2,1,8,0,5,0,0,0,4},
        {6,0,5,4,0,2,8,0,1}
    };

    if (solveSudoku(board)) {
        cout << "Solution:\n";
        printBoard(board);
    } else {
        cout << "No solution exists!\n";
    }

    return 0;
}
