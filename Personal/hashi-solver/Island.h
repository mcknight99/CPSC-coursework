#ifndef ISLAND_H
#define ISLAND_H

// helper class for the memory efficient map based implementation

#include <iostream>

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class Island
{
public:
    // Constructor for an island object
    // @param row: the row of the island
    // @param col: the column of the island
    // @param value: the value of the island
    Island(int row, int col, int value) : row(row), col(col), value(value), bridges(0, 0, 0, 0) {}

    // Basic variable getters; row, col, and value are immutable after construction
    int getRow() { return row; }
    int getCol() { return col; }
    int getValue() { return value; }
    std::tuple<int, int, int, int> getBridges() { return bridges; }

    // get the number of bridges in a single direction
    int getBridgeByDir(Direction dir) { return dir == Direction::UP ? std::get<0>(bridges) : 
                                               dir == Direction::RIGHT ? std::get<1>(bridges) : 
                                               dir == Direction::DOWN ? std::get<2>(bridges) : 
                                               std::get<3>(bridges); }

    // get the total number of bridges connected to the island
    int getTotalBridges() { return std::get<0>(bridges) + std::get<1>(bridges) + std::get<2>(bridges) + std::get<3>(bridges); }

    // remaining value is the value of the island minus the number of bridges connected to it
    int getRemainingValue() { return value - getTotalBridges(); }

    // add a bridge (if possible) in the direction dir, returns true if a bridge was added, false otherwise
    bool addBridge(Direction dir) { return setBridge(dir, getBridgeByDir(dir) + 1); }

    // remove a bridge (if possible) in the direction dir, returns true if a bridge was removed, false otherwise
    bool removeBridge(Direction dir) { return setBridge(dir, getBridgeByDir(dir) - 1); }

    // setter override for a single direction of bridges. can only be set to 0, 1, or 2. return true if the bridge was set, false otherwise
    bool setBridge(Direction dir, int bridges) { 
        if (bridges < 0 || bridges > 2) {
            return false;
        }
        if (dir == Direction::UP) {
            std::get<0>(this->bridges) = bridges;
        } else if (dir == Direction::RIGHT) {
            std::get<1>(this->bridges) = bridges;
        } else if (dir == Direction::DOWN) {
            std::get<2>(this->bridges) = bridges;
        } else {
            std::get<3>(this->bridges) = bridges;
        }
        return true;
    }

    // setter override for each direction of bridges
    void setBridges(int up, int right, int down, int left);

    // friend function to print the island
    friend std::ostream &operator<<(std::ostream &os, const Island &island) {
        os << "Island at (" << island.row << ", " << island.col << ") with value " << island.value << " and bridges <" << std::get<0>(island.bridges) << "U, " << std::get<1>(island.bridges) << "R, " << std::get<2>(island.bridges) << "D, " << std::get<3>(island.bridges) << "L>";
        return os;
    }

private:
    int row;
    int col;
    int value;
    // bridges = <up, down, left, right>
    // bridges can be 0-2 and in each direction (up, right, down, left). we need to store each of these individually
    std::tuple<int, int, int, int> bridges;
};

#endif // ISLAND_H