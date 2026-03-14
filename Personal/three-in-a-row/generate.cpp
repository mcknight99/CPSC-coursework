#include "generate.h"

std::tuple<int,int,Tile> randomLocationAndColor(Board* b) {
    int seed = b->getSeed();
    int genstep = b->getGenStep();
    //std::cout << "Generating tile at step " << genstep << " with seed " << seed << std::endl;
    std::mt19937 gen(seed + genstep);
    std::uniform_int_distribution<> disRow(0, b->getRows() - 1);
    std::uniform_int_distribution<> disCol(0, b->getCols() - 1);
    std::uniform_int_distribution<> disColor(0, 1);
    Tile color = (disColor(gen) == 0) ? Tile::BLACK : Tile::WHITE;
    b->incrementGenStep();
    //std::cout << "Generated tile at (" << disRow(gen) << ", " << disCol(gen) << ") with color " << (color == Tile::BLACK ? "BLACK" : "WHITE") << std::endl;
    return {disRow(gen), disCol(gen), color};
}

int countTiles(Board b) {
    int count = 0;
    for (int i = 0; i < b.getRows(); ++i) {
        for (int j = 0; j < b.getCols(); ++j) {
            if (b.getPos(i, j) != Tile::EMPTY) {
                ++count;
            }
        }
    }
    return count;
}

Board generate(int r, int c, float density, std::string seed) {
    return generate(r, c, density, Helper::gen_seeded_int(seed));
}

Board generate(int r, int c, float density, int s){
    Board board(r,c,s);
    std::cout << "Seed: " << board.getSeed() << "" << std::endl;
    int area = r * c;
    int numTiles = static_cast<int>(area * density);
    std::cout << "Generating " << numTiles << " tiles from density " << density << std::endl;

    do {
        board = Board(r,c,s, board.getGenStep() + 1); // Reset the board with the same seed and increment genstep
        while (countTiles(board) < numTiles) {
            auto [row, col, color] = randomLocationAndColor(&board);
            board.setPos(row, col, color);
        }
    } while(!validate(board,false));

    return board;
}

bool validate(Board b, bool testComplete) {
    // check that the board has none of the same value 3 in a row of black or white
    int rows = b.getRows();
    int cols = b.getCols();

    // Check rows for three 
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 2; ++j) {
            if (b.getPos(i, j) == b.getPos(i, j + 1) && b.getPos(i, j + 1) == b.getPos(i, j + 2) && b.getPos(i,j) != Tile::EMPTY) {
                std::cout<<"Three in a row at ("<<i<<", "<<j<<")"<<std::endl;
                return false;  // Found three tiles in a row horizontally
            }
        }
    }

    // Check columns for three 
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows - 2; ++i) {
            if (b.getPos(i, j) == b.getPos(i + 1, j) && b.getPos(i + 1, j) == b.getPos(i + 2, j) && b.getPos(i,j) != Tile::EMPTY) {
                std::cout<<"Three in a row at ("<<i<<", "<<j<<")"<<std::endl;
                return false;  // Found three tiles in a row vertically
            }
        }
    }

    // check that the board is entirely complete if testComplete is true
    if (testComplete) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (b.getPos(i, j) == Tile::EMPTY) {
                    return false;  // Found an empty position
                }
            }
        }
    }

    return true;  // Board is valid
}

