#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <unordered_map>

using Position = std::pair<int, int>;

class Player {
public:
    std::string name;
    std::string color;
    std::vector<std::vector<char>> personalBoard; // Player's view of the board
    std::unordered_map<char, bool> potentialLootIslands;

    Player() = default;
    Player(std::string name, std::string color, const std::vector<std::vector<char>>& board);

    void setGuess(Position center, int radius, bool answer);
    void setDig(Position island);
    void printBoard() const; // Debugging function
    void printPlayer() const;
};

#endif
