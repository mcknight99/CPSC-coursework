#ifndef HASHI_ARRAY_IMPLEMENTATION_H
#define HASHI_ARRAY_IMPLEMENTATION_H

#include "HashiBoard.h"

class HashiArrayImplementation : public HashiBoard {
private:
    int rows;
    int cols;
    std::vector<std::vector<Tile>> board;

public:
    HashiArrayImplementation(std::vector<std::vector<Tile>> board);
    void printBoard() const override;
    bool addBridge(int row, int col, int dir) override;
    bool removeBridge(int row, int col, int dir) override;
    Tile getTile(int row, int col) const override;
    int getRows() const override;
    int getCols() const override;
    int getIslandValue(int row, int col) const override;
    int getIslandBridgeByDir(int row, int col, int dir) const override;
    int getIslandTotalBridges(int row, int col) const override;
    void setIslandValue(int row, int col, int value) override;
    void setIslandBridgeByDir(int row, int col, int dir, int count) override;
    void setIslandTotalBridges(int row, int col, std::tuple<int,int,int,int> count) override;
    bool isBoardSolved() const override;
    bool solveBoard() override;
};

#endif // HASHI_ARRAY_IMPLEMENTATION_H
