#include "HashiMemImplementation.h"
#include <iostream>

HashiMemImplementation::HashiMemImplementation(std::vector<std::vector<Tile>> board) {
    // iterate through the entire board, if the tile is an island, add it to the map
    std::cout << "HashiMemImplementation: Constructor not implemented yet.\n";
}

// Print the board layout (memory-efficient version)
void HashiMemImplementation::printBoard() const {
    std::cout << "HashiMemImplementation: Print board not implemented yet.\n";
}

// Add a bridge between islands
bool HashiMemImplementation::addBridge(int row, int col, int dir) {
    std::cout << "HashiMemImplementation: Add bridge not implemented yet.\n";
    return false;
}

// Remove a bridge between islands
bool HashiMemImplementation::removeBridge(int row, int col, int dir) {
    std::cout << "HashiMemImplementation: Remove bridge not implemented yet.\n";
    return false;
}

// Get the tile at the given row and column
Tile HashiMemImplementation::getTile(int row, int col) const {
    std::cout << "HashiMemImplementation: Get tile not implemented yet.\n";
    return Tile::EMPTY;
}

// Get the total number of rows
int HashiMemImplementation::getRows() const {
    return rows;
}

// Get the total number of columns
int HashiMemImplementation::getCols() const {
    return cols;
}

// Get the value of an island at a specific location
int HashiMemImplementation::getIslandValue(int row, int col) const {
    std::cout << "HashiMemImplementation: Get island value not implemented yet.\n";
    return 0;
}

// Get the number of bridges from an island in a specific direction
int HashiMemImplementation::getIslandBridgeByDir(int row, int col, int dir) const {
    std::cout << "HashiMemImplementation: Get island bridge by direction not implemented yet.\n";
    return 0;
}

// Get the total number of bridges connected to an island
int HashiMemImplementation::getIslandTotalBridges(int row, int col) const {
    std::cout << "HashiMemImplementation: Get island total bridges not implemented yet.\n";
    return 0;
}

// Set the value of an island
void HashiMemImplementation::setIslandValue(int row, int col, int value) {
    std::cout << "HashiMemImplementation: Set island value not implemented yet.\n";
}

// Set the number of bridges from an island in a specific direction
void HashiMemImplementation::setIslandBridgeByDir(int row, int col, int dir, int count) {
    std::cout << "HashiMemImplementation: Set island bridge by direction not implemented yet.\n";
}

// Set the total number of bridges connected to an island
void HashiMemImplementation::setIslandTotalBridges(int row, int col, std::tuple<int,int,int,int> count) {
    std::cout << "HashiMemImplementation: Set island total bridges not implemented yet.\n";
}

// Check if the board is solved
bool HashiMemImplementation::isBoardSolved() const {
    std::cout << "HashiMemImplementation: Check if board is solved not implemented yet.\n";
    return false;
}

// Attempt to solve the board
bool HashiMemImplementation::solveBoard() {
    std::cout << "HashiMemImplementation: Solve board not implemented yet.\n";
    return false;
}
