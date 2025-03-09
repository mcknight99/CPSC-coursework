#ifndef HASHI_MEM_IMPLEMENTATION_H
#define HASHI_MEM_IMPLEMENTATION_H

#include "HashiBoard.h"
#include <map>

class HashiMemImplementation : public HashiBoard {
private:
    int rows, cols;
    // map of islands with the key being the row and column of the island
    std::map<std::pair<int, int>, Island> islands;

public:
    HashiMemImplementation(std::vector<std::vector<Tile>> board);
    void printBoard() const override;
    Tile getTile(int row, int col) const override;
    int getRows() const override;
    int getCols() const override;
    int getIslandValue(int row, int col) const override;
    int getIslandBridgeByDir(int row, int col, Direction dir) const override;
    int getIslandTotalBridges(int row, int col) const override;
    bool isBoardSolved() const override;
    bool solveBoard() override; // will contain recursive logic to solve ambiguous states once there are no more guaranteed moves, similar to my sudoku solver

//private:
    bool addBridge(int row, int col, Direction dir) override;
    bool removeBridge(int row, int col, Direction dir) override;
    void setIslandValue(int row, int col, int value) override;
    void setIslandBridgesByDir(int row, int col, Direction dir, int count) override;
    void setIslandTotalBridges(int row, int col, std::tuple<int,int,int,int> count) override;

    Tile isBridgeAtPos(int row, int col) const; // check if there is a bridge at a specific position using existing islands and their bridges
    Tile nextTileInDir(int row, int col, Direction dir) const; // get the next tile in a specific direction. Has bridge detection logic
    Island nextIslandInDir(int row, int col, Direction dir) const; // get the next island in a specific direction. Has bridge detection logic
    bool isIsland(int row, int col) const; // check if there is an island at a specific position
    Island getIsland(int row, int col) const; // get the island object at a specific position

    bool solveIsland(int row, int col) const; // solve an island using guaranteed moves
    void solveGuaranteedOdds(int row, int col) const; // solve by islands of even remaining value matching available bridges 
    void solveGuaranteedEvens(int row, int col) const; // solve by islands of odd remaining value matching available bridges
    void solveGuaranteedOnes(int row, int col) const; // solve by islands of value 1 with only one neighbor
};

#endif // HASHI_MEM_IMPLEMENTATION_H
