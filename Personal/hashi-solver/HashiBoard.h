#ifndef HASHI_BOARD_H
#define HASHI_BOARD_H

#include "Tile.h"
#include "Island.h"
#include <vector>

class HashiBoard {
public:
    virtual ~HashiBoard() = default;

    virtual void printBoard() const = 0;
    virtual bool addBridge(int row, int col, Direction dir) = 0;
    virtual bool removeBridge(int row, int col, Direction dir) = 0;
    virtual Tile getTile(int row, int col) const = 0;
    virtual int getRows() const = 0;
    virtual int getCols() const = 0;
    virtual int getIslandValue(int row, int col) const = 0;
    virtual int getIslandBridgeByDir(int row, int col, Direction dir) const = 0;
    virtual int getIslandTotalBridges(int row, int col) const = 0;
    virtual void setIslandValue(int row, int col, int value) = 0;
    virtual void setIslandBridgesByDir(int row, int col, Direction dir, int count) = 0;
    virtual void setIslandTotalBridges(int row, int col, std::tuple<int,int,int,int> count) = 0;
    virtual bool isBoardSolved() const = 0;
    virtual bool solveBoard() = 0;
};

#endif // HASHI_BOARD_H
