#ifndef TILE_H
#define TILE_H

#include <vector>
#include <unordered_map>
#include <iostream>

enum class Tile {
    EMPTY=0,
    ONE=1,
    TWO=2,
    THREE=3,
    FOUR=4,
    FIVE=5,
    SIX=6,
    SEVEN=7,
    EIGHT=8,
    H_BRIDGE=9,
    H_DOUBLE_BRIDGE=10,
    V_BRIDGE=11,
    V_DOUBLE_BRIDGE=12
};

class TileHelper {
public:
    static std::vector<std::vector<Tile>> convertToTiles(const std::vector<std::string>& board, const std::unordered_map<char, Tile>& charToTileFix) {
        std::vector<std::vector<Tile>> boardTiles;
        for (const auto& row : board) {
            std::vector<Tile> rowTiles;
            for (auto tile : row) {
                rowTiles.push_back(charToTileFix.at(tile));
            }
            boardTiles.push_back(rowTiles);
        }
        return boardTiles;
    }

    static char getTileChar(const Tile& tile) {
        // Implement this function based on your Tile class
        return ' '; // Placeholder
    }

    static std::ostream& printTile(std::ostream& os, const Tile& tile) {
        char tileChar = getTileChar(tile);
        os << tileChar;
        return os;
    }
};

#endif // TILE_H