#include <iostream>
#include <vector>
#include "HashiMemImplementation.h"
// #include "HashiArrayImplementation.h"

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
<<<<<<< Updated upstream
    vector<vector<Tile>> boardTiles;
    for (auto row : board)
    {
        std::vector<Tile> rowTiles;
        for (auto tile : row)
        {
            rowTiles.push_back(TileHelper::charToTile(tile));
        }
        boardTiles.push_back(rowTiles);
    }
=======
    vector<vector<Tile>> boardTiles = TileHelper::convertBoardToTiles(board);
>>>>>>> Stashed changes

    HashiMemImplementation *board1 = new HashiMemImplementation(boardTiles);
    // HashiBoard* board2 = new HashiArrayImplementation(10, 10);

    board1->printBoard();
    // board2->printBoard();

    std::cout << "whats at pos: 1 5: " << TileHelper::tileToChar(board1->getTile(1, 5)) << std::endl;
    std::cout << "is bridge at pos 1 5:" << TileHelper::tileToChar(board1->isBridgeAtPos(1, 5)) << std::endl;
    std::cout << "get bridges of island 1 1: " << board1->getIslandTotalBridges(1, 1) << std::endl;
    std::cout << "get bridges of island 1 1 in direction right: " << board1->getIslandBridgeByDir(1, 1, Direction::RIGHT) << std::endl;
    board1->addBridge(1, 1, Direction::RIGHT);
    board1->printBoard();
    std::cout << "whats at pos: 1 5: " << TileHelper::tileToChar(board1->getTile(1, 5)) << std::endl;
    std::cout << "is bridge at pos 1 5:" << TileHelper::tileToChar(board1->isBridgeAtPos(1, 5)) << std::endl;
    std::cout << "get bridges of island 1 1: " << board1->getIslandTotalBridges(1, 1) << std::endl;
    std::cout << "get bridges of island 1 1 in direction right: " << board1->getIslandBridgeByDir(1, 1, Direction::RIGHT) << std::endl;
    board1->addBridge(1, 1, Direction::RIGHT);
    board1->printBoard();
    std::cout << "whats at pos: 1 5: " << TileHelper::tileToChar(board1->getTile(1, 5)) << std::endl;
    std::cout << "is bridge at pos 1 5:" << TileHelper::tileToChar(board1->isBridgeAtPos(1, 5)) << std::endl;
    std::cout << "get bridges of island 1 1: " << board1->getIslandTotalBridges(1, 1) << std::endl;
    std::cout << "get bridges of island 1 1 in direction right: " << board1->getIslandBridgeByDir(1, 1, Direction::RIGHT) << std::endl;

    std::cout << board1->getIsland(1, 1) << std::endl;

    delete board1;
    // delete board2;
    return 0;
}
