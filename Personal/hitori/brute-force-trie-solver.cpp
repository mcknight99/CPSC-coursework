// a similar approach to the sudoku solver where it tries all valid combinations and returns all valid solutions and trims invalid branches

#include <iostream>
#include <vector>

using namespace std;

// hitori grid struct, each cell is <int, char> where int is the number and char is the color (B black, W white, U uncolored)
struct Cell {
    int number;
    char color; // 'W' for white, 'B' for black
};

struct Hitori {
    vector<vector<Cell>> grid;
};


int main() {
    vector<vector<int>> hitori = {
        {12,3,6,3,10,11,9,9,4,6,1,1},
        {3,5,1,2,12,9,6,6,9,8,11,2},
        {3,3,5,12,4,1,9,5,12,2,1,11},
        {5,10,11,1,12,9,1,10,13,9,6,8},
        {7,9,10,6,3,1,4,12,9,1,12,10},
        {5,7,5,2,4,4,1,12,10,9,3,12},
        {10,6,6,9,5,8,12,2,1,1,12,10},
        {2,12,10,8,10,7,12,5,5,11,4,6},
        {9,11,1,5,7,6,6,1,7,10,5,9},
        {1,2,8,10,8,3,5,6,7,12,9,6},
        {9,12,3,7,8,3,10,9,6,12,5,1},
        {12,10,3,11,7,12,3,4,6,6,7,11}
    };

    // create a hitori struct and fill it with the grid
    Hitori hitoriStruct;
    hitoriStruct.grid.resize(hitori.size(), vector<Cell>(hitori[0].size()));
    for (int i = 0; i < hitori.size(); i++) {
        for (int j = 0; j < hitori[i].size(); j++) {
            hitoriStruct.grid[i][j].number = hitori[i][j];
            hitoriStruct.grid[i][j].color = 'U'; // all cells are uncolored by default
        }
    }


}



// function to check if the current grid is valid
bool isValid(Hitori& hitori) {
    // check for duplicate W numbers in rows and columns
    for (int i = 0; i < hitori.grid.size(); i++) {
        vector<bool> rowCheck(hitori.grid[i].size(), false);
        vector<bool> colCheck(hitori.grid.size(), false);
        for (int j = 0; j < hitori.grid[i].size(); j++) {
            if (hitori.grid[i][j].color == 'W') {
                if (rowCheck[hitori.grid[i][j].number]) return false;
                rowCheck[hitori.grid[i][j].number] = true;
            }
            if (hitori.grid[j][i].color == 'W') {
                if (colCheck[hitori.grid[j][i].number]) return false;
                colCheck[hitori.grid[j][i].number] = true;
            }
        }
    }

    // check for adjacent black cells
    for (int i = 0; i < hitori.grid.size(); i++) {
        for (int j = 0; j < hitori.grid[i].size(); j++) {
            if (hitori.grid[i][j].color == 'B') {
                if ((i > 0 && hitori.grid[i - 1][j].color == 'B') || (j > 0 && hitori.grid[i][j - 1].color == 'B')) {
                    return false;
                }
            }
        }
    }

    // check that all white cells are connected
    for (int i = 0; i < hitori.grid.size(); i++) {
        for (int j = 0; j < hitori.grid[i].size(); j++) {
            if (hitori.grid[i][j].color == 'W') {
                // check if the cell is connected to another white cell
                bool connected = false;
                // an error will probably occur when checking the edges of the grid, so we need to check if the cell is not on the edge of the grid at some point
                if (i > 0 && hitori.grid[i - 1][j].color == 'W') connected = true;
                if (j > 0 && hitori.grid[i][j - 1].color == 'W') connected = true;
                if (i < hitori.grid.size() - 1 && hitori.grid[i + 1][j].color == 'W') connected = true;
                if (j < hitori.grid[i].size() - 1 && hitori.grid[i][j + 1].color == 'W') connected = true;
                if (!connected) return false;
            }
        }
    }

    return true;
}