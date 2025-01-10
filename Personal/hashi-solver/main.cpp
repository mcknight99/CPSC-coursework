#include <iostream>
#include <vector>
#include "HashiMemImplementation.h"
#include "HashiArrayImplementation.h"

using namespace std;

int main()
{
    cout << "Hello World" << endl;

    vector<vector<char>> board = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '3'},
        {' ', ' ', '4', ' ', ' ', '5', ' ', '3', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '2'},
        {' ', ' ', ' ', '3', ' ', '6', ' ', '5', ' ', ' '},
        {' ', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', '2', ' ', '3', ' ', '3', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', '3', ' ', ' ', ' ', ' ', ' ', ' ', '2'},
        {' ', '3', ' ', '2', ' ', '3', ' ', ' ', '2', ' '},

    };

    // Convert the board to Tiles
    vector<vector<Tile>> boardTiles;
    TileHelper th;
    for (auto row : board)
    {
        std::vector<Tile> rowTiles;
        for (auto tile : row)
        {
            rowTiles.push_back(th.charToTile[tile]);
        }
        boardTiles.push_back(rowTiles);
    }

    HashiBoard *board1 = new HashiMemImplementation(boardTiles);
    // HashiBoard* board2 = new HashiArrayImplementation(10, 10);

    board1->printBoard();
    // board2->printBoard();

    delete board1;
    // delete board2;
    return 0;
}
