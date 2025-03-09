#ifndef BOARD_H
#define BOARD_H

#include "player.h"
#include <unordered_map>

class Board {
public:
    std::vector<std::vector<char>> originalBoard;
    std::unordered_map<char, Position> islandPositions; // Map from IslandID to Position
    std::unordered_map<std::string, Player*> playersByName;
    std::unordered_map<std::string, Player*> playersByColor;

    Board(const std::vector<std::vector<char>>& board);
    
    void addPlayer(const std::string& name, const std::string& color);
    void doHint(const std::string& playerInfo, char islandLetter, int radius, bool answer);
    void doDig(const std::string& playerInfo, char islandLetter);
    void printPlayerBoard(const std::string& playerInfo) const;
    void printBoard() const;
    void printPlayer(const std::string& playerInfo) const;
};

#endif
