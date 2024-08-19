#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 4;


// Function to generate a random integer between min and max (inclusive)
int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function to print the grid
void printGrid(const vector<vector<int>>& grid) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << grid[i][j] << "\t";
        }
        cout << endl;
    }
}

// Function to check if all rows, columns, and diagonals sum up to the same value
bool checkMagicSquare(const vector<vector<int>>& grid) {
    int sum = 0;

    // Check rows
    for (int i = 0; i < SIZE; ++i) {
        int rowSum = 0;
        for (int j = 0; j < SIZE; ++j) {
            rowSum += grid[i][j];
        }
        if (i == 0) {
            sum = rowSum;
        } else if (rowSum != sum) {
            return false;
        }
    }

    // Check columns
    for (int j = 0; j < SIZE; ++j) {
        int colSum = 0;
        for (int i = 0; i < SIZE; ++i) {
            colSum += grid[i][j];
        }
        if (colSum != sum) {
            return false;
        }
    }

    // Check main diagonal
    int diagSum = 0;
    for (int i = 0; i < SIZE; ++i) {
        diagSum += grid[i][i];
    }
    if (diagSum != sum) {
        return false;
    }

    // Check anti-diagonal
    int antiDiagSum = 0;
    for (int i = 0; i < SIZE; ++i) {
        antiDiagSum += grid[i][SIZE - 1 - i];
    }
    if (antiDiagSum != sum) {
        return false;
    }

    return true;
}

int main() {
    int iterations = 1;
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<vector<int>> grid(SIZE, vector<int>(SIZE));

    while (true) {
        // Fill the grid with random values
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                grid[i][j] = randomInt(1, 99); // Assuming the range of random values is 1 to 16
            }
        }

        // Check if it forms a magic square
        if (checkMagicSquare(grid)) {
            cout << "Magic Square Found!" << endl;
            cout << "The Magic Square:" << endl;
            printGrid(grid);
            break; // Exit the loop if magic square is found
        } else {
            //system("CLS");
            cout << iterations <<endl;
            ++iterations;
        }
    }

    return 0;
}

