#include "board.h"
#include <iostream>

Board::Board(const std::vector<std::vector<char>>& board) : originalBoard(board) {
    // Populate islandPositions
    for (int r = 0; r < board.size(); ++r) {
        for (int c = 0; c < board[r].size(); ++c) {
            char tile = board[r][c];
            if (tile != '$') { // Supply islands are not part of loot islands
                islandPositions[tile] = {r, c};
            }
        }
    }
}

void Board::addPlayer(const std::string& name, const std::string& color) {
    Player* p = new Player(name, color, originalBoard);
    playersByName[name] = p;
    playersByColor[color] = p;
}

void Board::doHint(const std::string& playerInfo, char islandLetter, int radius, bool answer) {
    // convert islandLetter to uppercase if it is lowercase
    if (islandLetter >= 'a' && islandLetter <= 'z') {
        islandLetter = islandLetter - 'a' + 'A';
    }
    if (islandPositions.find(islandLetter) != islandPositions.end()) {
        Position islandPos = islandPositions[islandLetter];
        if (playersByName.find(playerInfo) != playersByName.end()) {
            playersByName[playerInfo]->setGuess(islandPos, radius, answer);
        } else if (playersByColor.find(playerInfo) != playersByColor.end()) {
            playersByColor[playerInfo]->setGuess(islandPos, radius, answer);
        } else {
            std::cerr << "Player \"" << playerInfo << "\" not found." << std::endl;
        }
    } else {
        std::cerr << "Island \"" << islandLetter << "\" not found." << std::endl;
    }
}

void Board::doDig(const std::string& playerInfo, char islandLetter) {
    // convert islandLetter to uppercase if it is lowercase
    if (islandLetter >= 'a' && islandLetter <= 'z') {
        islandLetter = islandLetter - 'a' + 'A';
    }
    if (islandPositions.find(islandLetter) != islandPositions.end()) {
        Position islandPos = islandPositions[islandLetter];
        if (playersByName.find(playerInfo) != playersByName.end()) {
            playersByName[playerInfo]->setDig(islandPos);
        } else if (playersByColor.find(playerInfo) != playersByColor.end()) {
            playersByColor[playerInfo]->setDig(islandPos);
        } else {
            std::cerr << "Player \"" << playerInfo << "\" not found." << std::endl;
        }
    } else {
        std::cerr << "Island \"" << islandLetter << "\" not found." << std::endl;
    }
}

void Board::printPlayerBoard(const std::string& playerInfo) const {
    if (playersByName.find(playerInfo) != playersByName.end()) {
        playersByName.at(playerInfo)->printBoard();
    } else if (playersByColor.find(playerInfo) != playersByColor.end()) {
        playersByColor.at(playerInfo)->printBoard();
    } else {
        std::cerr << "Player \"" << playerInfo << "\" not found." << std::endl;
    }
}

void Board::printBoard() const {
    std::cout << "Original board:\n";
    for (const auto& row : originalBoard) {
        for (char cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

void Board::printPlayer(const std::string& playerInfo) const {
    if (playersByName.find(playerInfo) != playersByName.end()) {
        playersByName.at(playerInfo)->printPlayer();
    } else if (playersByColor.find(playerInfo) != playersByColor.end()) {
        playersByColor.at(playerInfo)->printPlayer();
    } else {
        std::cerr << "Player \"" << playerInfo << "\" not found." << std::endl;
    }
}