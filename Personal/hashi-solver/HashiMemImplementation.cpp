#include "HashiMemImplementation.h"
#include <iostream>

HashiMemImplementation::HashiMemImplementation(std::vector<std::vector<Tile>> board)
{
    // iterate through the entire board, if the tile is an island, add it to the map
    for (size_t i = 0; i < board.size(); i++)
    {
        for (size_t j = 0; j < board[i].size(); j++)
        {
            if (TileHelper::isIsland(board[i][j]))
            {
                islands[std::make_pair(i, j)] = Island(i, j, static_cast<int>(board[i][j]));
            }
        }
    }
    rows = board.size();
    cols = board[0].size();
}

// Print the board layout (memory-efficient version)
void HashiMemImplementation::printBoard() const
{
    std::cout << "rows and cols: " << rows << ", " << cols << std::endl;
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (isIsland(row, col))
            {
                Island t = getIsland(row, col);
                std::cout << t.getValue();
            }
            else if (isBridgeAtPos(row, col) != Tile::EMPTY)
            {
                std::cout << TileHelper::tileToChar(isBridgeAtPos(row, col));
            }
            else
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

// Add a bridge between islands
bool HashiMemImplementation::addBridge(int row, int col, Direction dir)
{
    bool safe = true;
    Island i1, i2;
    if (isIsland(row, col))
    {
        i1 = getIsland(row, col);
    }
    else
    {
        std::cerr << "Error: No island at position (" << row << ", " << col << ").\n";
        safe = false;
    }
    if (nextIslandInDir(row, col, dir).getRow() != -1)
    {
        i2 = nextIslandInDir(row, col, dir);
    }
    else
    {
        std::cerr << "Error: No island in direction " << dir << " from position (" << row << ", " << col << ").\n";
        safe = false;
    }
    if (safe)
    {
        bool added = i1.addBridge(dir) && i2.addBridge(Island::oppositeDir(dir)); // if 1 fails then 2 wont run due to short circuiting
        if (!added)
        {
            safe = false;
            std::cerr << "Error: Unable to add bridge between islands at (" << row << ", " << col << ") and (" << i2.getRow() << ", " << i2.getCol() << "). Potentially both have two bridges\n";
        } else {
            std::cout<<"Bridge added between islands at (" << row << ", " << col << ") and (" << i2.getRow() << ", " << i2.getCol() << ").\n";
        }
    }
    return safe;
}

// Remove a bridge between islands
bool HashiMemImplementation::removeBridge(int row, int col, Direction dir)
{
    bool safe = true;
    Island i1, i2;
    if (isIsland(row, col))
    {
        i1 = getIsland(row, col);
    }
    else
    {
        std::cerr << "Error: No island at position (" << row << ", " << col << ").\n";
        safe = false;
    }
    if (nextIslandInDir(row, col, dir).getRow() != -1)
    {
        i2 = nextIslandInDir(row, col, dir);
    }
    else
    {
        std::cerr << "Error: No island in direction " << dir << " from position (" << row << ", " << col << ").\n";
        safe = false;
    }
    if (safe)
    {
        if (!i1.removeBridge(dir) || !i2.removeBridge(dir))
        {
            safe = false;
            std::cerr << "Error: Unable to remove bridge between islands at (" << row << ", " << col << ") and (" << i2.getRow() << ", " << i2.getCol() << "). Potentially both have zero bridges\n";
        } else {
            std::cout<<"Bridge removed between islands at (" << row << ", " << col << ") and (" << i2.getRow() << ", " << i2.getCol() << ").\n";
        }
    }
    return safe;
}

// Get the tile at the given row and column. Includes bridges
Tile HashiMemImplementation::getTile(int row, int col) const
{
    Tile t = Tile::EMPTY;
    if (isIsland(row, col))
    {
        Island i = getIsland(row, col);
        t = static_cast<Tile>(i.getValue());
    }
    else if (isBridgeAtPos(row, col) != Tile::EMPTY)
    {
        std::cout << "Need to determine bridge position";
        t = Tile::ERROR;
    }
    return t;
}

// Get the total number of rows
int HashiMemImplementation::getRows() const
{
    return rows;
}

// Get the total number of columns
int HashiMemImplementation::getCols() const
{
    return cols;
}

// Get the value of an island at a specific location
int HashiMemImplementation::getIslandValue(int row, int col) const
{
    int out = 0;
    if (isIsland(row, col))
    {
        Island i = getIsland(row, col);
        out = i.getValue();
    }
    return out;
}

// Get the number of bridges from an island in a specific direction
int HashiMemImplementation::getIslandBridgeByDir(int row, int col, Direction dir) const
{
    int out = 0;
    if (isIsland(row, col))
    {
        Island i = getIsland(row, col);
        out = i.getBridgeByDir(dir);
    }
    return out;
}

// Get the total number of bridges connected to an island
int HashiMemImplementation::getIslandTotalBridges(int row, int col) const
{
    int out = 0;
    if (isIsland(row, col))
    {
        Island i = getIsland(row, col);
        out = i.getTotalBridges();
    }
    return out;
}

// Set the value of an island
void HashiMemImplementation::setIslandValue(int row, int col, int value)
{
    std::cout << "HashiMemImplementation: Set island value not implemented yet.\n";
}

// Set the number of bridges from an island in a specific direction
void HashiMemImplementation::setIslandBridgesByDir(int row, int col, Direction dir, int count)
{
    std::cout << "HashiMemImplementation: Set island bridge by direction not implemented yet.\n";
}

// Set the total number of bridges connected to an island
void HashiMemImplementation::setIslandTotalBridges(int row, int col, std::tuple<int, int, int, int> count)
{
    std::cout << "HashiMemImplementation: Set island total bridges not implemented yet.\n";
}

// Check if the board is solved
bool HashiMemImplementation::isBoardSolved() const
{
    std::cout << "HashiMemImplementation: Check if board is solved not implemented yet.\n";
    return false;
}

// Attempt to solve the board
bool HashiMemImplementation::solveBoard()
{
    std::cout << "HashiMemImplementation: Solve board not implemented yet.\n";
    return false;
}

// Solve an island using guaranteed moves
bool HashiMemImplementation::solveIsland(int row, int col) const
{
    std::cout << "HashiMemImplementation: Solve island not implemented yet.\n";
    return false;
}

// Solve by islands of even remaining value matching available bridges
void HashiMemImplementation::solveGuaranteedOdds(int row, int col) const
{
    std::cout << "HashiMemImplementation: Solve guaranteed odds not implemented yet.\n";
}

// Solve by islands of odd remaining value matching available bridges
void HashiMemImplementation::solveGuaranteedEvens(int row, int col) const
{
    std::cout << "HashiMemImplementation: Solve guaranteed evens not implemented yet.\n";
}

// Solve by islands of value 1 with only one neighbor
void HashiMemImplementation::solveGuaranteedOnes(int row, int col) const
{
    std::cout << "HashiMemImplementation: Solve guaranteed ones not implemented yet.\n";
}

// Check if there is a bridge at a specific position using existing islands and their bridges
// returns empty if there is no bridge, returns H_BRIDGE, V_BRIDGE, H_DOUBLE_BRIDGE, or V_DOUBLE_BRIDGE if there is a bridge
Tile HashiMemImplementation::isBridgeAtPos(int row, int col) const
{
    // check for next tile in each dir. if there is an island up and down with a bridge down and up respectively, return true
    // or if there is an island right and left with a bridge left and right respectively, return true
    Tile bridge = Tile::EMPTY;

    // lets find an island in each direction
    Island up = nextIslandInDir(row, col, Direction::UP);
    Island down = nextIslandInDir(row, col, Direction::DOWN);
    Island left = nextIslandInDir(row, col, Direction::LEFT);
    Island right = nextIslandInDir(row, col, Direction::RIGHT);

    // check if there is a bridge between up and down or left and right
    if (up.getRow() != -1 && down.getRow() != -1)
    {
        if (up.getBridgeByDir(Direction::DOWN) == 1 && down.getBridgeByDir(Direction::UP) == 1)
        {
            bridge = Tile::V_BRIDGE;
        }
        else if (up.getBridgeByDir(Direction::DOWN) == 2 && down.getBridgeByDir(Direction::UP) == 2)
        {
            bridge = Tile::V_DOUBLE_BRIDGE;
        }
        else
        {
            bridge = Tile::EMPTY;
        }
    }
    if (left.getCol() != -1 && right.getCol() != -1)
    {
        if (left.getBridgeByDir(Direction::RIGHT) == 1 && right.getBridgeByDir(Direction::LEFT) == 1)
        {
            bridge = Tile::H_BRIDGE;
        }
        else if (left.getBridgeByDir(Direction::RIGHT) == 2 && right.getBridgeByDir(Direction::LEFT) == 2)
        {
            bridge = Tile::H_DOUBLE_BRIDGE;
        }
        else
        {
            bridge = Tile::EMPTY;
        }
    }

    return bridge;
}

// Get the next tile in a specific direction. Has bridge detection logic, does not return empty tiles
Tile HashiMemImplementation::nextTileInDir(int row, int col, Direction dir) const
{
    Tile t = Tile::ERROR;
    do
    {
        if (dir == Direction::UP)
        {
            row--;
        }
        else if (dir == Direction::RIGHT)
        {
            col++;
        }
        else if (dir == Direction::DOWN)
        {
            row++;
        }
        else
        {
            col--;
        }
        t = getTile(row, col);
    } while (t != Tile::EMPTY);
    return t;
}

// Get the next island in a specific direction. Ignores bridges
Island HashiMemImplementation::nextIslandInDir(int row, int col, Direction dir) const
{
    Island i = Island();
    do
    {
        if (dir == Direction::UP)
        {
            row--;
        }
        else if (dir == Direction::RIGHT)
        {
            col++;
        }
        else if (dir == Direction::DOWN)
        {
            row++;
        }
        else
        {
            col--;
        }
        if (isIsland(row, col))
        {
            i = getIsland(row, col);
            break;
        }
    } while (row >= 0 && row < rows && col >= 0 && col < cols);
    return i;
}

// Check if there is an island at a specific position
bool HashiMemImplementation::isIsland(int row, int col) const
{
    return islands.find(std::make_pair(row, col)) != islands.end();
}

// Get the island object at a specific position
Island HashiMemImplementation::getIsland(int row, int col) const
{
    return islands.at(std::make_pair(row, col));
}