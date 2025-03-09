#ifndef TILE_H
#define TILE_H

<<<<<<< Updated upstream
#include <map>
#include <iostream>
=======
#include <vector>
#include <unordered_map>
>>>>>>> Stashed changes

enum class Tile
{
    ERROR=-1,
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

const class TileHelper
{
    public:
<<<<<<< Updated upstream
    static const char tileToChar(Tile in) {
        switch(in) {
            case Tile::EMPTY: return ' ';
            case Tile::V_BRIDGE: return '\'';
            case Tile::V_DOUBLE_BRIDGE: return '"';
            case Tile::H_BRIDGE: return '-';
            case Tile::H_DOUBLE_BRIDGE: return '=';
            case Tile::ONE: return '1';
            case Tile::TWO: return '2';
            case Tile::THREE: return '3';
            case Tile::FOUR: return '4';
            case Tile::FIVE: return '5';
            case Tile::SIX: return '6';
            case Tile::SEVEN: return '7';
            case Tile::EIGHT: return '8';
            default: return 'E';
        }
    }

    static const Tile charToTile(char in) {
        switch(in) {
            case ' ': return Tile::EMPTY;
            case '\'': return Tile::V_BRIDGE;
            case '"': return Tile::V_DOUBLE_BRIDGE;
            case '-': return Tile::H_BRIDGE;
            case '=': return Tile::H_DOUBLE_BRIDGE;
            case '1': return Tile::ONE;
            case '2': return Tile::TWO;
            case '3': return Tile::THREE;
            case '4': return Tile::FOUR;
            case '5': return Tile::FIVE;
            case '6': return Tile::SIX;
            case '7': return Tile::SEVEN;
            case '8': return Tile::EIGHT;
            default: return Tile::ERROR;
        }
=======
    std::unordered_map<Tile, char> tileToChar = {
        {Tile::EMPTY, ' '},
        {Tile::V_BRIDGE, '\''},
        {Tile::V_DOUBLE_BRIDGE, '"'},
        {Tile::H_BRIDGE, '-'},
        {Tile::H_DOUBLE_BRIDGE, '='},
        {Tile::ONE, '1'},
        {Tile::TWO, '2'},
        {Tile::THREE, '3'},
        {Tile::FOUR, '4'},
        {Tile::FIVE, '5'},
        {Tile::SIX, '6'},
        {Tile::SEVEN, '7'},
        {Tile::EIGHT, '8'}};

    std::unordered_map<char, Tile> charToTile = {
        {' ', Tile::EMPTY},
        {'\'', Tile::V_BRIDGE},
        {'"', Tile::V_DOUBLE_BRIDGE},
        {'-', Tile::H_BRIDGE},
        {'=', Tile::H_DOUBLE_BRIDGE},
        {'1', Tile::ONE},
        {'2', Tile::TWO},
        {'3', Tile::THREE},
        {'4', Tile::FOUR},
        {'5', Tile::FIVE},
        {'6', Tile::SIX},
        {'7', Tile::SEVEN},
        {'8', Tile::EIGHT}};

    // function to get the char representation of a tile
    const char getTileChar(Tile tile)
    {
        return tileToChar[tile];
>>>>>>> Stashed changes
    }

    // function to determine if the tile is an island (1-8)
    static const bool isIsland(Tile tile)
    {
        return tile >= Tile::ONE && tile <= Tile::EIGHT;
    }
    const bool isIsland(char tile)
    {
        return isIsland(charToTile(tile));
    }

    // function to determine if the tile is a bridge
    static const bool isBridge(Tile tile)
    {
        return tile == Tile::V_BRIDGE || tile == Tile::V_DOUBLE_BRIDGE || tile == Tile::H_BRIDGE || tile == Tile::H_DOUBLE_BRIDGE;
    }
    const bool isBridge(char tile)
    {
        return isBridge(charToTile(tile));
    }

<<<<<<< Updated upstream
    // friend function to print the island
    friend std::ostream &operator<<(std::ostream &os, const Tile &tile) {
        os << tileToChar(tile);
        return os;
    }

    
=======
    const std::vector<std::vector<Tile>> convertBoardToTiles(std::vector<std::vector<char>> board)
    {
        // I did something weird with statics and consts and whatnot with this helper class and i cant figure it out
        // this should be identical to charToTile
        
        std::vector<std::vector<Tile>> boardTiles;
        for (auto row : board)
        {
            std::vector<Tile> rowTiles;
            for (auto tile : row)
            {
                rowTiles.push_back(charToTile[tile]);
            }
            boardTiles.push_back(rowTiles);
        }
        return boardTiles;
    }

    // friend function to cout a tile
    // or nevermind because it was throwing a lot of errors?
    // std::ostream &operator<<(std::ostream &os, const Tile &tile)
    // {
    //     char tileChar = getTileChar(tile);
    //     os << tileChar;
    //     return os;
    // }
>>>>>>> Stashed changes
};

#endif // TILE_H