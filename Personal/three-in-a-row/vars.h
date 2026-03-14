#ifndef VARS_H
#define VARS_H

#include <vector>
#include <iostream>
#include <random>
#include <climits>

static const std::string ANSI_BLACK = "\u001B[30;40m";
static const std::string ANSI_WHITE = "\u001B[37;47m";
static const std::string ANSI_RESET = "\u001B[0m";

enum class Tile {
    EMPTY = '.',
    BLACK = 'b',
    WHITE = 'w',
    ERROR = '!'
};

class Board{
    private:
        std::vector<std::vector<Tile>> board; // " " is empty, "b" is black, "w" is white
        int rows, cols;
        int seed;
        int genstep = 0;

    public:
        Board(int r, int c, int s = 0, int g = 0) {
            rows = r;
            cols = c;
            seed = s;
            genstep = g;
            board.resize(rows, std::vector<Tile>(cols, Tile::EMPTY));
        }

        std::vector<std::vector<Tile>> getBoard() const {
            return board;
        }

        int getRows() const {
            return rows;
        }

        int getCols() const {
            return cols;
        }

        int getSeed() const {
            return seed;
        }

        void setSeed(int s) {
            seed = s;
        }

        int getGenStep() const {
            return genstep;
        }

        void incrementGenStep() {
            genstep++;
        }

        void setPos(int r, int c, Tile val) {
            if (r >= 0 && r < rows && c >= 0 && c < cols) {
                board[r][c] = val;
            }
        }

        Tile getPos(int r, int c) const {
            if (r >= 0 && r < rows && c >= 0 && c < cols) {
                return board[r][c];
            }
            return Tile::ERROR;  // Invalid position
        }

        void printBoard(bool fancy = false) const {
            for (const auto& row : board) {
                for (const auto& cell : row) {
                    if (fancy) {
                        char c = static_cast<char>(cell);
                        // use terminal highlighting with colors
                        if (cell == Tile::BLACK) {
                            std::cout << ANSI_BLACK << c << ANSI_RESET;
                        } else if (cell == Tile::WHITE) {
                            std::cout << ANSI_WHITE << c << ANSI_RESET;
                        } else {
                            std::cout << c;
                        }
                    } else {
                        std::cout << static_cast<char>(cell);
                    }
                }
                std::cout << std::endl;
            }
        }

};

class Helper{
    public:
    static int gen_seeded_int(std::string seedstr = "")
    {
        std::hash<std::string> hasher;
        int seed = hasher(seedstr);
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
        return dist(gen);
    }

};


#endif