#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum class Tile
{
    EMPTY,
    V_BRIDGE,
    V_DOUBLE_BRIDGE,
    H_BRIDGE,
    H_DOUBLE_BRIDGE,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT
};

map<Tile, char> tileToChar = {
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

map<char, Tile> charToTile = {
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

map<Direction, pair<int, int>> directionToPair = {
    {Direction::UP, {-1, 0}},
    {Direction::RIGHT, {0, 1}},
    {Direction::DOWN, {1, 0}},
    {Direction::LEFT, {0, -1}}};

// Prototypes

std::pair<int, int> nextNonBlankSpace(int row, int col, vector<vector<Tile>> board, Direction dir);
std::pair<int, int> nextIsland(int row, int col, vector<vector<Tile>> board, Direction dir);
map<Direction, pair<int, int>> getAdjacentIslands(int row, int col, vector<vector<Tile>> board);
int islandRemainingValue(int row, int col, vector<vector<Tile>> board);
std::map<Direction, Tile> getAdjacentTiles(int row, int col, vector<vector<Tile>> board);
int availableBridges(int row, int col, vector<vector<Tile>> board, Direction dir);
void fillLine(int startRow, int startCol, int endRow, int endCol, vector<vector<Tile>> &board, Tile tile);
bool solveSpread(int row, int col, vector<vector<Tile>> board);
bool solveEquality(int row, int col, vector<vector<Tile>> board);
vector<vector<Tile>> solveGuaranteedMoves(vector<vector<Tile>> board);
void printBoard(vector<vector<Tile>> board);
Tile whatsAtPosition(int row, int col, vector<vector<Tile>> board);
bool solveIslandEquality(vector<vector<Tile>> &board);

int main()
{
    vector<vector<char>> board = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '3'},
        {' ', ' ', '4', ' ', ' ', '5', ' ', '3', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '2'},
        {' ', ' ', ' ', '3', ' ', '6', ' ', '5', ' ', ' '},
        {' ', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', '2', ' ', '3', ' ', '3', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', '3', ' ', ' ', ' ', ' ', ' ', ' ', '2'},
        {' ', '3', ' ', '2', ' ', '3', ' ', ' ', '2', ' '},

    };

    // Convert the board to Tiles
    vector<vector<Tile>> boardTiles;
    for (auto row : board)
    {
        vector<Tile> rowTiles;
        for (auto tile : row)
        {
            rowTiles.push_back(charToTile[tile]);
        }
        boardTiles.push_back(rowTiles);
    }

    cout << "Original board" << endl;
    printBoard(boardTiles);

    // testing function solveIslandEquality
    solveIslandEquality(boardTiles);
    cout<<"After solving island equality"<<endl;
    printBoard(boardTiles);
}

// Returns the row and column of the next non-blank space in the direction dir from the input row and col
// Returns -1, -1 if there is no non-blank space in the direction
// @param row: the row of the starting position
// @param col: the column of the starting position
// @param board: the board to search for the next non-blank space
// @param dir: the direction to search for the next non-blank space
// @return pair<int, int>: the row and column of the next non-blank space
std::pair<int, int> nextNonBlankSpace(int row, int col, vector<vector<Tile>> board, Direction dir)
{
    if (dir == Direction::UP)
    {
        for (int i = row - 1; i >= 0; --i)
        {
            if (board[i][col] != Tile::EMPTY)
            {
                return make_pair(i, col);
            }
        }
    }
    else if (dir == Direction::RIGHT)
    {
        for (size_t i = col + 1; i < board[row].size(); ++i)
        {
            if (board[row][i] != Tile::EMPTY)
            {
                return make_pair(row, i);
            }
        }
    }
    else if (dir == Direction::DOWN)
    {
        for (size_t i = row + 1; i < board.size(); ++i)
        {
            if (board[i][col] != Tile::EMPTY)
            {
                return make_pair(i, col);
            }
        }
    }
    else if (dir == Direction::LEFT)
    {
        for (int i = col - 1; i >= 0; --i)
        {
            if (board[row][i] != Tile::EMPTY)
            {
                return make_pair(row, i);
            }
        }
    }
    return make_pair(-1, -1);
}

// Returns the row and column of the next island in the direction dir from the input row and col
// Returns -1, -1 if there is no island in the direction
// @param row: the row of the starting position
// @param col: the column of the starting position
// @param board: the board to search for the next island
// @param dir: the direction to search for the next island
// @return pair<int, int>: the row and column of the next island
std::pair<int, int> nextIsland(int row, int col, vector<vector<Tile>> board, Direction dir)
{
    set<Tile> nonIslandTiles = {Tile::EMPTY, Tile::V_BRIDGE, Tile::V_DOUBLE_BRIDGE, Tile::H_BRIDGE, Tile::H_DOUBLE_BRIDGE};
    if (dir == Direction::UP)
    {
        for (int i = row - 1; i >= 0; --i)
        {
            if (nonIslandTiles.find(board[i][col]) == nonIslandTiles.end())
            {
                return make_pair(i, col);
            }
        }
    }
    else if (dir == Direction::RIGHT)
    {
        for (size_t i = col + 1; i < board[row].size(); ++i)
        {
            if (nonIslandTiles.find(board[row][i]) == nonIslandTiles.end())
            {
                return make_pair(row, i);
            }
        }
    }
    else if (dir == Direction::DOWN)
    {
        for (size_t i = row + 1; i < board.size(); ++i)
        {
            if (nonIslandTiles.find(board[i][col]) == nonIslandTiles.end())
            {
                return make_pair(i, col);
            }
        }
    }
    else if (dir == Direction::LEFT)
    {
        for (int i = col - 1; i >= 0; --i)
        {
            if (nonIslandTiles.find(board[row][i]) == nonIslandTiles.end())
            {
                return make_pair(row, i);
            }
        }
    }
    return make_pair(-1, -1);
}

// Returns a map of the directions to the row and column of the adjacent islands
// @param row: the row of the starting position
// @param col: the column of the starting position
// @param board: the board to search for the adjacent islands
// @return map<Direction, pair<int, int>>: the directions to the row and column of the adjacent islands
map<Direction, pair<int, int>> getAdjacentIslands(int row, int col, vector<vector<Tile>> board)
{
    map<Direction, pair<int, int>> adjacentIslands;
    for (auto dir : directionToPair)
    {
        pair<int, int> next = nextIsland(row, col, board, dir.first);
        if (next.first != -1)
        {
            adjacentIslands[dir.first] = next;
        }
    }
    return adjacentIslands;
}

// Fills the line between the start and end positions with the tile
// Doesn't fill line inclusively for the ends, only fills the line between the start and end
// This simplifies the logic for creating bridges from one island to another
// @pre: startRow and endRow are the same or startCol and endCol are the same
// @param startRow: the row of the starting position
// @param startCol: the column of the starting position
// @param endRow: the row of the ending position
// @param endCol: the column of the ending position
// @param board: the board to fill the line
// @param tile: the tile to fill the line with
void fillLine(int startRow, int startCol, int endRow, int endCol, vector<vector<Tile>> &board, Tile tile)
{
    if (startRow == endRow)
    {
        for (int i = startCol + 1; i < endCol; ++i)
        {
            board[startRow][i] = tile;
        }
    }
    else if (startCol == endCol)
    {
        for (int i = startRow + 1; i < endRow; ++i)
        {
            board[i][startCol] = tile;
        }
    }
}

// Returns the remaining value of the island at the row and column
// The remaining value is the value of the island minus the bridges that exist connecting to it
// @param row: the row of the island
// @param col: the column of the island
// @param board: the board to search for the island
// @return int: the remaining value of the island
int islandRemainingValue(int row, int col, vector<vector<Tile>> board)
{
    int value = tileToChar[board[row][col]] - '0'; // Convert char to int with a really cool trick of subtracting the char '0' from the char representing the value

    int bridges = 0;
    // RIGHT
    if (board[row][col + 1] == Tile::H_BRIDGE)
    {
        ++bridges;
    }
    else if (board[row][col + 1] == Tile::H_DOUBLE_BRIDGE)
    {
        bridges += 2;
    }
    // LEFT
    if (board[row][col - 1] == Tile::H_BRIDGE)
    {
        ++bridges;
    }
    else if (board[row][col - 1] == Tile::H_DOUBLE_BRIDGE)
    {
        bridges += 2;
    }
    // UP
    if (board[row - 1][col] == Tile::V_BRIDGE)
    {
        ++bridges;
    }
    else if (board[row - 1][col] == Tile::V_DOUBLE_BRIDGE)
    {
        bridges += 2;
    }
    // DOWN
    if (board[row + 1][col] == Tile::V_BRIDGE)
    {
        ++bridges;
    }
    else if (board[row + 1][col] == Tile::V_DOUBLE_BRIDGE)
    {
        bridges += 2;
    }

    return value - bridges;
}

// Returns a map of the directions to the adjacent tiles
// @param row: the row of the starting position
// @param col: the column of the starting position
// @param board: the board to search for the adjacent tiles
// @return map<Direction, Tiles>: the directions to the adjacent tiles
std::map<Direction, Tile> getAdjacentTiles(int row, int col, vector<vector<Tile>> board)
{
    std::map<Direction, Tile> adjacentTiles;
    if (row - 1 >= 0)
    {
        adjacentTiles[Direction::UP] = board[row - 1][col];
    }
    if (static_cast<size_t>(col + 1) < board[row].size())
    {
        adjacentTiles[Direction::RIGHT] = board[row][col + 1];
    }
    if (static_cast<size_t>(row + 1) < board.size())
    {
        adjacentTiles[Direction::DOWN] = board[row + 1][col];
    }
    if (col - 1 >= 0)
    {
        adjacentTiles[Direction::LEFT] = board[row][col - 1];
    }
    return adjacentTiles;
}

// uses the immediate island in direction dir to determine the amount of bridges that can be placed, and subtracting existing bridges
// don't forget to take into account the ther bridge's remaining values if they can only take one more bridge
// the available bridges between two islands is:
// if the islands have two or more remaining value, cap the available bridges between the two at two
// subtract the existing bridges between the two islands from the available bridges between the two
// Returns the amount of bridges that can be placed between the island at row and col and the next island in direction dir
// @param row: the row of the island
// @param col: the column of the island
// @param board: the board to search for the island
// @param dir: the direction to search for the next island
// @return int: the amount of bridges that can be placed between the island and the next island
int availableBridges(int row, int col, vector<vector<Tile>> board, Direction dir)
{
    int availableBridges = 0;
    pair<int, int> next = nextIsland(row, col, board, dir);
    if (next.first != -1)
    {
        int remainingValue = islandRemainingValue(next.first, next.second, board);
        if (remainingValue >= 2)
        {
            availableBridges = 2;
        }
        else
        {
            availableBridges = remainingValue;
        }
    }
    // Subtract existing bridges
    if (board[row + directionToPair[dir].first][col + directionToPair[dir].second] == Tile::H_BRIDGE)
    {
        --availableBridges;
    }
    else if (board[row + directionToPair[dir].first][col + directionToPair[dir].second] == Tile::H_DOUBLE_BRIDGE)
    {
        availableBridges -= 2;
    }
    else if (board[row + directionToPair[dir].first][col + directionToPair[dir].second] == Tile::V_BRIDGE)
    {
        --availableBridges;
    }
    else if (board[row + directionToPair[dir].first][col + directionToPair[dir].second] == Tile::V_DOUBLE_BRIDGE)
    {
        availableBridges -= 2;
    }
    return availableBridges;
}

// Spread is a hard problem to solve because it doesn't fill an island but it "halfway" solves it (not actually halfway, but it solves the guaranteed bridges between islands)
// Spread is taking the available bridges between each adjacent island and comparing them to existing bridges and the remaining value of the island
// For example: the island is 3 and has 2 islands that can take 2 each, it is guaranteed to have one bridge to each island, but the third bridge is not guaranteed
// This function aims to solve the guaranteed "spread" bridges between islands
// If the island has a remaining value of N and the available bridges is N+1, then the island is guaranteed to have a spread of 1 bridge to each adjacent island of non-zero value
// but it has to include existing single bridges because they contribute to the spread as well as can disable the spread in some edge cases
// ACTUALLY i think it should take the original value rather than remaining value to solve spread, the edge cases come from trying to use the remaining value.
// ALTERNATIVELY use remaining value when we can ignore filled islands, but use the original value when we can't ignore filled islands
// Returns true if a change was made to the board, false otherwise
// @param row: the row of the island
// @param col: the column of the island
// @param board: the board to search for the island
bool solveSpread(int row, int col, vector<vector<Tile>> board)
{
    return false;
}

// // The solve equality function aims to find islands that have the same remaining value and available bridges
// // This function is easier to solve because we can place all of the bridges between all of the nearby islands
// // Returns true if a change was made to the board, false otherwise
// // @param row: the row of the island
// // @param col: the column of the island
// // @param board: the board to search for the island
// bool solveEquality(int row, int col, vector<vector<Tile>> board)
// {
//     map<Direction, pair<int, int>> adjacentIslands = getAdjacentIslands(row, col, board);
//     for (auto island : adjacentIslands)
//     {
//         int available = availableBridges(row, col, board, island.first);
//         int remainingValue = islandRemainingValue(island.second.first, island.second.second, board);
//         if (remainingValue == available)
//         {
//             // The island is guaranteed to have all of the bridges between the two islands
//             // Fill the line between the two islands
//         }
//     }
//     return false;
// }

// Solves the guaranteed moves on the board
// This function is the main function that will call the other functions to solve the board
// Returns the board with the guaranteed moves solved
// @param board: the board to solve
// @return vector<vector<Tile>>: the board with the guaranteed moves solved
vector<vector<Tile>> solveGuaranteedMoves(vector<vector<Tile>> board)
{
    // Start with islands that have the same amount of available bridges and remaining value (1 and 2)
    // Also do spread moves to minimize the moves to solve
    // if a change was made, reiterate in the same spot before moving onto the next cell (what if instead of iterating each cell we grab a list of each island and iterate through that)
    return board;
}

// Prints the board to the console
// @param board: the board to print
void printBoard(vector<vector<Tile>> board)
{
    for (auto row : board)
    {
        for (auto tile : row)
        {
            cout << tileToChar[tile];
        }
        cout << endl;
    }
}

// Returns the tile at the position row and col
// If the position is out of bounds, return Tile::EMPTY
// @param row: the row of the position
// @param col: the column of the position
// @param board: the board to search for the position
// @return Tile: the tile at the position
Tile whatsAtPosition(int row, int col, vector<vector<Tile>> board)
{
    if (row < 0 || col < 0 || static_cast<size_t>(row) >= board.size() || static_cast<size_t>(col) >= board[0].size())
    {
        return Tile::EMPTY;
    }
    else
    {
        return board[row][col];
    }
}

// Solve island equality: if an island has equal amount of remaining value and available bridges, then fill all available bridges
// Pass the board by reference so that the board is modified
// @param board: the board to solve
// @return bool: true if a change was made, false otherwise
bool solveIslandEquality(vector<vector<Tile>> &board)
{
    bool changeMade = false;
    for (size_t row = 0; row < board.size(); ++row)
    {
        for (size_t col = 0; col < board[row].size(); ++col)
        {
            if (board[row][col] != Tile::EMPTY && board[row][col] != Tile::V_BRIDGE && board[row][col] != Tile::V_DOUBLE_BRIDGE && board[row][col] != Tile::H_BRIDGE && board[row][col] != Tile::H_DOUBLE_BRIDGE)
            {
                
            }
        }
    }
    return changeMade;
}