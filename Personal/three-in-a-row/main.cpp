#include "generate.h"

int main() {
    std::string seed = "seeee";
    //std::cout << "Seed: \"" << seed << "\"" << std::endl;
    //int seed = -472629561;
    float d = 0.5;
    Board board = generate(7, 7, d, seed);
    board.printBoard(true);
    std::cout << "Rows: " << board.getRows() << ", Cols: " << board.getCols() << ", Seed: " << board.getSeed() << ", GenStep: " << board.getGenStep() << std::endl;

    return 0;
}