#ifndef TILE_H
#define TILE_H

#include <map>
#include <iostream>

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

class TileHelper
{
    public:
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

    // friend function to print the island
    friend std::ostream &operator<<(std::ostream &os, const Tile &tile) {
        os << tileToChar(tile);
        return os;
    }

    
};

#endif // TILE_H