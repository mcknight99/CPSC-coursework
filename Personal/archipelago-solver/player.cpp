#include "player.h"
#include <iostream>

Player::Player(std::string name, std::string color, const std::vector<std::vector<char>> &board)
    : name(name), color(color), personalBoard(board) {}

void Player::setGuess(Position center, int radius, bool answer)
{
    int rows = personalBoard.size();
    int cols = personalBoard[0].size();
    int islandRow = center.first;
    int islandCol = center.second;

    // Ensure center is in bounds
    if (islandRow < 0 || islandRow >= rows || islandCol < 0 || islandCol >= cols)
        return;

    // for all cells on the radius ring, set their entry in the potentialLootIslands map to existing value AND answer. if the cell doesn't exist in the map, set it to answer
    // for answers = false, set the cell to 'X'
    for (int r = islandRow - radius; r <= islandRow + radius; ++r)
    {
        for (int c = islandCol - radius; c <= islandCol + radius; ++c)
        { // for all cells on the radius ring, not within the radius ring
            if (r >= 0 && r < rows && c >= 0 && c < cols && (r == islandRow - radius || r == islandRow + radius || c == islandCol - radius || c == islandCol + radius))
            {
                if (potentialLootIslands.find(personalBoard[r][c]) != potentialLootIslands.end())
                {
                    potentialLootIslands[personalBoard[r][c]] = potentialLootIslands[personalBoard[r][c]] && answer; // ensures that the value is only true if it was true before and is true now and will never be true if it was ever false
                }
                else
                {
                    potentialLootIslands[personalBoard[r][c]] = answer;
                }
                personalBoard[r][c] = answer ? personalBoard[r][c] : 'X';
            }
        }
    }
}

void Player::setDig(Position island)
{
    int row = island.first;
    int col = island.second;

    if (row >= 0 && row < personalBoard.size() && col >= 0 && col < personalBoard[0].size())
    {
        personalBoard[row][col] = 'X'; // Mark as invalid
    }
}

void Player::printBoard() const
{
    std::cout << "Board for " << name << " (" << color << "):\n";
    for (const auto &row : personalBoard)
    {
        for (char cell : row)
        {
            if (cell == 'X' || cell == '$')
            { // if the cell is X or $ print a red highlight space and then an unformatted space
                std::cout << "\033[1;41m \033[0m ";
            }
            else if (potentialLootIslands.find(cell) != potentialLootIslands.end() && potentialLootIslands.at(cell))
            { // if the cell is in the potentialLootIslands map and is true, print as green bold text and then an unformatted space
                std::cout << "\033[1;32m" << cell << "\033[0m ";
            }
            else
            { // otherwise print the cell and an unformatted space
                std::cout << cell << " ";
            }
        }
        std::cout << std::endl;
    }
}

void Player::printPlayer() const
{
    std::cout << "Player: " << name << " (" << color << ")\n";
}