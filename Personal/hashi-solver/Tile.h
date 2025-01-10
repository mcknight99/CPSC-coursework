#ifndef TILE_H
#define TILE_H

#include <map>

enum class Tile
{
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

class TileHelper
{
    public:
    std::map<Tile, char> tileToChar = {
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

    std::map<char, Tile> charToTile = {
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
    }

    // function to determine if the tile is an island (1-8)
    const bool isIsland(Tile tile)
    {
        return tile >= Tile::ONE && tile <= Tile::EIGHT;
    }
    const bool isIsland(char tile)
    {
        return isIsland(charToTile[tile]);
    }

    // function to determine if the tile is a bridge
    const bool isBridge(Tile tile)
    {
        return tile == Tile::V_BRIDGE || tile == Tile::V_DOUBLE_BRIDGE || tile == Tile::H_BRIDGE || tile == Tile::H_DOUBLE_BRIDGE;
    }
    const bool isBridge(char tile)
    {
        return isBridge(charToTile[tile]);
    }

    // friend function to cout a tile
    // or nevermind because it was throwing a lot of errors?
    // std::ostream &operator<<(std::ostream &os, const Tile &tile)
    // {
    //     char tileChar = getTileChar(tile);
    //     os << tileChar;
    //     return os;
    // }
};

#endif // TILE_H