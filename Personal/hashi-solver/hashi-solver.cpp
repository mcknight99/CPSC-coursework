#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum class Tiles {
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

map<Tiles, char> tileToChar = {
    {Tiles::EMPTY, ' '},
    {Tiles::V_BRIDGE, '\''},
    {Tiles::V_DOUBLE_BRIDGE, '"'},
    {Tiles::H_BRIDGE, '-'},
    {Tiles::H_DOUBLE_BRIDGE, '='},    
    {Tiles::ONE, '1'},
    {Tiles::TWO, '2'},
    {Tiles::THREE, '3'},
    {Tiles::FOUR, '4'},
    {Tiles::FIVE, '5'},
    {Tiles::SIX, '6'},
    {Tiles::SEVEN, '7'},
    {Tiles::EIGHT, '8'}
};

map<Direction, pair<int, int>> directionToPair = {
    {Direction::UP, {-1, 0}},
    {Direction::RIGHT, {0, 1}},
    {Direction::DOWN, {1, 0}},
    {Direction::LEFT, {0, -1}}
};

// Prototypes
std::pair<int,int> nextNonBlankSpace(int row, int col, vector<vector<Tiles>> board, Direction dir);
void fillLine(int startRow, int startCol, int endRow, int endCol, vector<vector<Tiles>> &board, Tiles tile);
int islandRemainingValue(int row, int col, vector<vector<Tiles>> board);
std::map<Direction, Tiles> getAdjacentTiles(int row, int col, vector<vector<Tiles>> board);
vector<vector<Tiles>> solveGuaranteedMoves(vector<vector<Tiles>> board);


int main() {

    vector<vector<char>> board = {
        {' ', ' ', ' ',' ',' ',' ',' ',' ',' ',' '},
        {' ', '4', ' ',' ',' ',' ',' ',' ',' ','3'},
        {' ', ' ', '4',' ',' ','5',' ','3',' ',' '},
        {' ', ' ', ' ',' ',' ',' ',' ',' ',' ','2'},
        {' ', ' ', ' ','3',' ','6',' ','5',' ',' '},
        {' ', '4', ' ',' ',' ',' ',' ',' ',' ',' '},
        {' ', ' ', ' ','2',' ','3',' ','3',' ',' '},
        {' ', ' ', ' ',' ',' ',' ',' ',' ',' ',' '},
        {' ', ' ', '3',' ',' ',' ',' ',' ',' ','2'},
        {' ', '3', ' ','2',' ','3',' ',' ','2',' '},

        };

    vector<vector<Tiles>> boardTiles;
    for(auto row : board) {
        vector<Tiles> rowTiles;
        for(auto tile : row) {
            rowTiles.push_back(static_cast<Tiles>(tile - '0'));
        }
        boardTiles.push_back(rowTiles);
    }

    for(auto row : boardTiles) {
        for(auto tile : row) {
            cout<< tileToChar[tile];
        }
        cout<< endl;
    }

    // Testing functions
    cout<< "Testing functions" << endl;
    cout<< "islandRemainingValue(1, 1, boardTiles): " << islandRemainingValue(1, 1, boardTiles) << endl;
    cout<< "islandRemainingValue(2, 5, boardTiles): " << islandRemainingValue(2, 5, boardTiles) << endl;

    cout<< "getAdjacentTiles(1, 1, boardTiles): " << endl;
    std::map<Direction, Tiles> adjacentTiles = getAdjacentTiles(1, 1, boardTiles);
    for(auto tile : adjacentTiles) {
        cout<< tileToChar[tile.second] << " ";
    }
    cout<< endl;

    cout<< "getAdjacentTiles(2, 5, boardTiles): " << endl;
    adjacentTiles = getAdjacentTiles(2, 5, boardTiles);
    for(auto tile : adjacentTiles) {
        cout<< tileToChar[tile.second] << " ";
    }
    cout<< endl;

    cout<< "nextNonBlankSpace(1, 1, boardTiles, Direction::UP): " << endl;
    pair<int,int> nextSpace = nextNonBlankSpace(1, 1, boardTiles, Direction::UP);
    cout<< nextSpace.first << " " << nextSpace.second << endl;
    cout<< "nextNonBlankSpace(1, 1, boardTiles, Direction::RIGHT): " << endl;
    nextSpace = nextNonBlankSpace(1, 1, boardTiles, Direction::RIGHT);
    cout<< nextSpace.first << " " << nextSpace.second << endl;
    cout<< "nextNonBlankSpace(1, 1, boardTiles, Direction::DOWN): " << endl;
    nextSpace = nextNonBlankSpace(1, 1, boardTiles, Direction::DOWN);
    cout<< nextSpace.first << " " << nextSpace.second << endl;
    cout<< "nextNonBlankSpace(1, 1, boardTiles, Direction::LEFT): " << endl;
    nextSpace = nextNonBlankSpace(1, 1, boardTiles, Direction::LEFT);
    cout<< nextSpace.first << " " << nextSpace.second << endl;

    cout<< "nextNonBlankSpace(2, 5, boardTiles, Direction::UP): " << endl;
    nextSpace = nextNonBlankSpace(2, 5, boardTiles, Direction::UP);
    cout<< nextSpace.first << " " << nextSpace.second << endl;
    cout<< "nextNonBlankSpace(2, 5, boardTiles, Direction::RIGHT): " << endl;
    nextSpace = nextNonBlankSpace(2, 5, boardTiles, Direction::RIGHT);
    cout<< nextSpace.first << " " << nextSpace.second << endl;
    cout<< "nextNonBlankSpace(2, 5, boardTiles, Direction::DOWN): " << endl;
    nextSpace = nextNonBlankSpace(2, 5, boardTiles, Direction::DOWN);
    cout<< nextSpace.first << " " << nextSpace.second << endl;
    cout<< "nextNonBlankSpace(2, 5, boardTiles, Direction::LEFT): " << endl;
    nextSpace = nextNonBlankSpace(2, 5, boardTiles, Direction::LEFT);
    cout<< nextSpace.first << " " << nextSpace.second << endl;

    cout<< "fillLine(1, 1, 1, 9, boardTiles, Tiles::H_DOUBLE_BRIDGE): " << endl;
    fillLine(1, 1, 1, 9, boardTiles, Tiles::H_DOUBLE_BRIDGE);
    cout<< "fillLine(1, 1, 5, 1, boardTiles, Tiles::V_DOUBLE_BRIDGE): " << endl;
    fillLine(1,1,5,1, boardTiles, Tiles::V_DOUBLE_BRIDGE);
    cout<< "fillLine(2, 5, 2, 7, boardTiles, Tiles::H_BRIDGE): " << endl;
    fillLine(2, 5, 2, 7, boardTiles, Tiles::H_BRIDGE);
    cout<< "fillLine(2, 5, 4, 5, boardTiles, Tiles::V_BRIDGE): " << endl;
    fillLine(2, 5, 4, 5, boardTiles, Tiles::V_BRIDGE);
    

    for(auto row : boardTiles) {
        for(auto tile : row) {
            cout<< tileToChar[tile];
        }
        cout<< endl;
    }
    
}

std::pair<int,int> nextNonBlankSpace(int row, int col, vector<vector<Tiles>> board, Direction dir) {
    if(dir == Direction::UP) {
        for(int i = row - 1; i >= 0; --i) {
            if(board[i][col] != Tiles::EMPTY) {
                return make_pair(i, col);
            }
        }
    } else if(dir == Direction::RIGHT) {
        for(size_t i = col + 1; i < board[row].size(); ++i) {
            if(board[row][i] != Tiles::EMPTY) {
                return make_pair(row, i);
            }
        }
    } else if(dir == Direction::DOWN) {
        for(size_t i = row + 1; i < board.size(); ++i) {
            if(board[i][col] != Tiles::EMPTY) {
                return make_pair(i, col);
            }
        }
    } else if(dir == Direction::LEFT) {
        for(int i = col - 1; i >= 0; --i) {
            if(board[row][i] != Tiles::EMPTY) {
                return make_pair(row, i);
            }
        }
    }
    return make_pair(-1, -1);
}

std::pair<int, int> nextIsland(int row, int col, vector<vector<Tiles>> board, Direction dir) {
    set<Tiles> nonIslandTiles = {Tiles::EMPTY, Tiles::V_BRIDGE, Tiles::V_DOUBLE_BRIDGE, Tiles::H_BRIDGE, Tiles::H_DOUBLE_BRIDGE};
    if(dir == Direction::UP) {
        for(int i = row - 1; i >= 0; --i) {
            if(nonIslandTiles.find(board[i][col]) == nonIslandTiles.end()) {
                return make_pair(i, col);
            }
        }
    } else if(dir == Direction::RIGHT) {
        for(size_t i = col + 1; i < board[row].size(); ++i) {
            if(nonIslandTiles.find(board[row][i]) == nonIslandTiles.end()) {
                return make_pair(row, i);
            }
        }
    } else if(dir == Direction::DOWN) {
        for(size_t i = row + 1; i < board.size(); ++i) {
            if(nonIslandTiles.find(board[i][col]) == nonIslandTiles.end()) {
                return make_pair(i, col);
            }
        }
    } else if(dir == Direction::LEFT) {
        for(int i = col - 1; i >= 0; --i) {
            if(nonIslandTiles.find(board[row][i]) == nonIslandTiles.end()) {
                return make_pair(row, i);
            }
        }
    }
    return make_pair(-1, -1);
}

map<Direction, pair<int, int>> getAdjacentIslands(int row, int col, vector<vector<Tiles>> board) {
    map<Direction, pair<int, int>> adjacentIslands;
    for(auto dir : directionToPair) {
        pair<int, int> next = nextIsland(row, col, board, dir.first);
        if(next.first != -1) {
            adjacentIslands[dir.first] = next;
        }
    }
    return adjacentIslands;
}

// Doesn't fill line inclusively for the ends, only fills the line between the start and end
// This simplifies the logic for creating bridges from one island to another
void fillLine(int startRow, int startCol, int endRow, int endCol, vector<vector<Tiles>> &board, Tiles tile) {
    if(startRow == endRow) {
        for(int i = startCol + 1; i < endCol; ++i) {
            board[startRow][i] = tile;
        }
    } else if(startCol == endCol) {
        for(int i = startRow + 1; i < endRow; ++i) {
            board[i][startCol] = tile;
        }
    }
}

int islandRemainingValue(int row, int col, vector<vector<Tiles>> board) {
    int value = tileToChar[board[row][col]] - '0'; // Convert char to int with a really cool trick of subtracting the char '0' from the char representing the value

    int bridges = 0;
    // RIGHT
    if(board[row][col+1] == Tiles::H_BRIDGE) {
        ++bridges;
    } else if (board[row][col+1] == Tiles::H_DOUBLE_BRIDGE) {
        bridges += 2;
    }
    // LEFT
    if(board[row][col-1] == Tiles::H_BRIDGE) {
        ++bridges;
    } else if (board[row][col-1] == Tiles::H_DOUBLE_BRIDGE) {
        bridges += 2;
    }
    // UP
    if(board[row-1][col] == Tiles::V_BRIDGE) {
        ++bridges;
    } else if (board[row-1][col] == Tiles::V_DOUBLE_BRIDGE) {
        bridges += 2;
    }
    // DOWN
    if(board[row+1][col] == Tiles::V_BRIDGE) {
        ++bridges;
    } else if (board[row+1][col] == Tiles::V_DOUBLE_BRIDGE) {
        bridges += 2;
    }
    

    return value - bridges;
}

std::map<Direction, Tiles> getAdjacentTiles(int row, int col, vector<vector<Tiles>> board) {
    std::map<Direction, Tiles> adjacentTiles;
    if(row - 1 >= 0) {
        adjacentTiles[Direction::UP] = board[row - 1][col];
    }
    if(static_cast<size_t>(col + 1) < board[row].size()) {
        adjacentTiles[Direction::RIGHT] = board[row][col + 1];
    }
    if(static_cast<size_t>(row + 1) < board.size()) {
        adjacentTiles[Direction::DOWN] = board[row + 1][col];
    }
    if(col - 1 >= 0) {
        adjacentTiles[Direction::LEFT] = board[row][col - 1];
    }
    return adjacentTiles;
}

// uses the immediate island in direction dir to determine the amount of bridges that can be placed, and subtracting existing bridges
// don't forget to take into account the ther bridge's remaining values if they can only take one more bridge
// the available bridges between two islands is:
    // if the islands have two or more remaining value, cap the available bridges between the two at two
    // subtract the existing bridges between the two islands from the available bridges between the two
int availableBridges(int row, int col, vector<vector<Tiles>> board, Direction dir) {
    int availableBridges = 0;
    pair<int, int> next = nextIsland(row, col, board, dir);
    if(next.first != -1) {
        int remainingValue = islandRemainingValue(next.first, next.second, board);
        if(remainingValue >= 2) {
            availableBridges = 2;
        } else {
            availableBridges = remainingValue;
        }
    }
    // Subtract existing bridges
    if(board[row+directionToPair[dir].first][col+directionToPair[dir].second] == Tiles::H_BRIDGE) {
        --availableBridges;
    } else if(board[row+directionToPair[dir].first][col+directionToPair[dir].second] == Tiles::H_DOUBLE_BRIDGE) {
        availableBridges -= 2;
    } else if(board[row+directionToPair[dir].first][col+directionToPair[dir].second] == Tiles::V_BRIDGE) {
        --availableBridges;
    } else if(board[row+directionToPair[dir].first][col+directionToPair[dir].second] == Tiles::V_DOUBLE_BRIDGE) {
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
bool solveSpread(int row, int col, vector<vector<Tiles>> board) {
}

// The solve equality function aims to find islands that have the same remaining value and available bridges
// This function is easier to solve because we can place all of the bridges between all of the nearby islands
bool solveEquality(int row, int col, vector<vector<Tiles>> board) {
    map<Direction, pair<int, int>> adjacentIslands = getAdjacentIslands(row, col, board);
    for(auto island : adjacentIslands) {
        int available = availableBridges(row, col, board, island.first);
        int remainingValue = islandRemainingValue(island.second.first, island.second.second, board);
        if(remainingValue == available) {
            // The island is guaranteed to have all of the bridges between the two islands
            // Fill the line between the two islands
            
        }
    }
    return false;
}


vector<vector<Tiles>> solveGuaranteedMoves(vector<vector<Tiles>> board) {
    // Start with islands that have the same amount of available bridges and remaining value (1 and 2)
    // Also do spread moves to minimize the moves to solve
    // if a change was made, reiterate in the same spot before moving onto the next cell (what if instead of iterating each cell we grab a list of each island and iterate through that)

}

