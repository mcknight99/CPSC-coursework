#include "HashiArrayImplementation.h"
#include <iostream>

HashiArrayImplementation::HashiArrayImplementation(std::vector<std::vector<Tile>> board)
    : board(board) {}

// Print the board layout (2D array version)
void HashiArrayImplementation::printBoard() const {
    std::cout << "HashiArrayImplementation: Print board not implemented yet.\n";
}

// Add a bridge between islands
bool HashiArrayImplementation::addBridge(int row, int col, int dir) {
    std::cout << "HashiArrayImplementation: Add bridge not implemented yet.\n";
    return false;
}

// Remove a bridge between islands
bool HashiArrayImplementation::removeBridge(int row, int col, int dir) {
    std::cout << "HashiArrayImplementation: Remove bridge not implemented yet.\n";
    return false;
}

// Get the tile at the given row and column
Tile HashiArrayImplementation::getTile(int row, int col) const {
    std::cout << "HashiArrayImplementation: Get tile not implemented yet.\n";
    return board[row][col];
}

// Get the total number of rows
int HashiArrayImplementation::getRows() const {
    return rows;
}

// Get the total number of columns
int HashiArrayImplementation::getCols() const {
    return cols;
}

// Get the value of an island at a specific location
int HashiArrayImplementation::getIslandValue(int row, int col) const {
    std::cout << "HashiArrayImplementation: Get island value not implemented yet.\n";
    return 0;
}

// Get the number of bridges from an island in a specific direction
int HashiArrayImplementation::getIslandBridgeByDir(int row, int col, int dir) const {
    std::cout << "HashiArrayImplementation: Get island bridge by direction not implemented yet.\n";
    return 0;
}

// Get the total number of bridges connected to an island
int HashiArrayImplementation::getIslandTotalBridges(int row, int col) const {
    std::cout << "HashiArrayImplementation: Get island total bridges not implemented yet.\n";
    return 0;
}

// Set the value of an island
void HashiArrayImplementation::setIslandValue(int row, int col, int value) {
    std::cout << "HashiArrayImplementation: Set island value not implemented yet.\n";
}

// Set the number of bridges from an island in a specific direction
void HashiArrayImplementation::setIslandBridgeByDir(int row, int col, int dir, int count) {
    std::cout << "HashiArrayImplementation: Set island bridge by direction not implemented yet.\n";
}

// Set the total number of bridges connected to an island
void HashiArrayImplementation::setIslandTotalBridges(int row, int col, std::tuple<int,int,int,int> count) {
    std::cout << "HashiArrayImplementation: Set island total bridges not implemented yet.\n";
}

// Check if the board is solved
bool HashiArrayImplementation::isBoardSolved() const {
    std::cout << "HashiArrayImplementation: Check if board is solved not implemented yet.\n";
    return false;
}

// Attempt to solve the board
bool HashiArrayImplementation::solveBoard() {
    std::cout << "HashiArrayImplementation: Solve board not implemented yet.\n";
    return false;
}
