#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <windows.h> // For Sleep function
// for proper output formatting and spacing
#include <iomanip>
#include <chrono> // For timing the program

#include "cellenum.h"

using namespace std;

// Define a coordinate pair
typedef pair<int, int> Coord;

// Define a piece as a list of coordinate pairs
typedef vector<Coord> Piece;

// Define the board as a 2D vector of strings
typedef vector<vector<CellID>> Board;

// Color modifiers for terminal
struct colors
{
    const std::string DEFAULT_COLOR = "\033[0m";       // white text
    const std::string YELLOW_BOLD = "\033[1;33m";      // yellow text
    const std::string TEAL = "\033[1;36m";             // teal text
    const std::string BACKGROUND_COLOR = "\u001B[40m"; // black highlight background
    const std::string RED = "\x1B[31m";
    const std::string GREEN = "\x1B[32m";
    const std::string YELLOW = "\x1B[33m";
    const std::string BLUE = "\x1B[34m";
    const std::string MAGENTA = "\x1B[35m";
    const std::string CYAN = "\x1B[36m";
    const std::string WHITE = "\x1B[37m";
    const std::string GRAY = "\x1B[1;30m";
    const std::string GREEN_BACKGROUND = "\033[3;42;30m";
    const std::string YELLOW_BACKGROUND = "\033[3;43;30m";
    const std::string BLUE_BACKGROUND = "\033[3;44;30m";
    const std::string CYAN_BACKGROUND = "\033[3;104;30m";
    const std::string GRAY_BACKGROUND = "\033[3;100;30m";
    const std::string RED_BACKGROUND = "\033[3;101;30m";
    const std::string MAGENTA_BACKGROUND = "\033[3;45;30m";
};

// pair cell id's to colors to print
std::vector<std::pair<CellID, std::string>> idColors = {
    {CellID::P1, colors().GREEN_BACKGROUND},
    {CellID::P2, colors().YELLOW_BACKGROUND},
    {CellID::P3, colors().BLUE_BACKGROUND},
    {CellID::P4, colors().CYAN_BACKGROUND},
    {CellID::P5, colors().GRAY_BACKGROUND},
    {CellID::P6, colors().RED_BACKGROUND},
    {CellID::P7, colors().MAGENTA_BACKGROUND},
    {CellID::P8, colors().RED},
    {CellID::P9, colors().GREEN},
    {CellID::P10, colors().CYAN}};


// Function to print board
void printBoard(const Board &board)
{
    for (const auto &row : board)
    {
        for (const auto &cell : row)
        {
            if (to_string(cell).at(0) == 'P')
            {
                for (const auto &idColor : idColors)
                {
                    if (idColor.first == cell)
                    {
                        cout << idColor.second;
                    }
                }
            }
            if(cell==CellID::wall){
                cout << colors().GRAY;
            }
            cout << setw(5) << left << to_string(cell);
            cout << colors().DEFAULT_COLOR;
        }
        cout << colors().DEFAULT_COLOR << " | " << endl;
    }
    cout << "-------------------------------------" << endl;
}

// Function to rotate a piece 90 degrees clockwise
Piece rotatePiece(const Piece &piece)
{
    Piece rotated;
    for (const auto &coord : piece)
    {
        rotated.emplace_back(-coord.second, coord.first);
    }
    return rotated;
}

// Function to mirror a piece horizontally
Piece mirrorPiece(const Piece &piece)
{
    Piece mirrored;
    for (const auto &coord : piece)
    {
        mirrored.emplace_back(coord.first, -coord.second);
    }
    return mirrored;
}

// Function to check if a piece can be placed on the board at a specific position
bool canPlacePiece(const Board &board, const Piece &piece, int row, int col, const CellID &month, const CellID &day, const CellID &dayOfWeek)
{
    for (const auto &coord : piece)
    {
        int newRow = row + coord.first;
        int newCol = col + coord.second;
        // Check bounds
        if (newRow < 0 || newRow >= static_cast<int>(board.size()) || newCol < 0 || newCol >= static_cast<int>(board[0].size()))
        {
            return false;
        }
        // Check if the piece overlaps with the user's input or a wall or another piece
        CellID cell = board[newRow][newCol];
        if (cell == month || cell == day || cell == dayOfWeek || cell == CellID::wall || to_string(cell).at(0) == 'P')
        {
            return false;
        }
    }
    return true;
}

// Function to place a piece on the board
void placePiece(Board &board, const Piece &piece, int row, int col, const CellID &pieceID)
{
    for (const auto &coord : piece)
    {
        int newRow = row + coord.first;
        int newCol = col + coord.second;
        board[newRow][newCol] = pieceID;
    }
}

// Function to remove a piece from the board
// Replacing a cell with "open" is okay because pieces will only be placed when it won't overlap important cells. unimportant cells get overwritten, so the data there is irrelevant
void removePiece(Board &board, const Piece &piece, int row, int col)
{
    for (const auto &coord : piece)
    {
        int newRow = row + coord.first;
        int newCol = col + coord.second;
        board[newRow][newCol] = CellID::open;
    }
}

// Backtracking function to solve the board
bool solveBoard(Board &board, vector<Piece> &pieces, int index, const CellID &month, const CellID &day, const CellID &dayOfWeek)
{
    //Sleep(500);
    //std::cout << "Index: " << index << std::endl;
    printBoard(board);
    if (index == static_cast<int>(pieces.size()))
    {
        // All pieces have been placed
        for (auto row : board)
        {
            for (auto cell : row)
            {
                if (cell == CellID::open)
                {
                    return false;
                }
            }
        }
        // No open spots
        return true;
    }

    Piece &piece = pieces[index];

    for (int row = 0; row < static_cast<int>(board.size()); ++row)
    {
        for (int col = 0; col < static_cast<int>(board[0].size()); ++col)
        {
            // Try all transformations of the piece (original, rotated, mirrored, etc.)
            for (int i = 0; i < 4; ++i)
            { // Rotate the piece 4 times
                if (canPlacePiece(board, piece, row, col, month, day, dayOfWeek))
                {
                    placePiece(board, piece, row, col, stringToCellID("P" + to_string(index + 1)));
                    if (solveBoard(board, pieces, index + 1, month, day, dayOfWeek))
                    {
                        return true;
                    }
                    removePiece(board, piece, row, col);
                }
                piece = rotatePiece(piece);
            }
            piece = mirrorPiece(piece); // Try mirrored versions
        }
    }

    return false;
}

// Main function
int main()
{

    auto start = std::chrono::high_resolution_clock::now();

    /*
    Board board = {
        {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "wall"},
        {"Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "wall"},
        {"1", "2", "3", "4", "5", "6", "7"},
        {"8", "9", "10", "11", "12", "13", "14"},
        {"15", "16", "17", "18", "19", "20", "21"},
        {"22", "23", "24", "25", "26", "27", "28"},
        {"29", "30", "31", "Sun", "Mon", "Tue", "Wed"},
        {"wall", "wall", "wall", "wall", "Thu", "Fri", "Sat"}};
    */

    Board board = {
        {CellID::Jan, CellID::Feb, CellID::Mar, CellID::Apr, CellID::May, CellID::Jun, CellID::wall},
        {CellID::Jul, CellID::Aug, CellID::Sep, CellID::Oct, CellID::Nov, CellID::Dec, CellID::wall},
        {CellID::one, CellID::two, CellID::three, CellID::four, CellID::five, CellID::six, CellID::seven},
        {CellID::eight, CellID::nine, CellID::ten, CellID::eleven, CellID::twelve, CellID::thirteen, CellID::fourteen},
        {CellID::fifteen, CellID::sixteen, CellID::seventeen, CellID::eighteen, CellID::nineteen, CellID::twenty, CellID::twentyone},
        {CellID::twentytwo, CellID::twentythree, CellID::twentyfour, CellID::twentyfive, CellID::twentysix, CellID::twentyseven, CellID::twentyeight},
        {CellID::twentynine, CellID::thirty, CellID::thirtyone, CellID::Sun, CellID::Mon, CellID::Tue, CellID::Wed},
        {CellID::wall, CellID::wall, CellID::wall, CellID::wall, CellID::Thu, CellID::Fri, CellID::Sat}};

    vector<Piece> pieces = {
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}},         // P1 // Line piece
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}}, // P2 // Tall L piece
        {{0, 0}, {1, 0}, {2, 0}, {0, 1}},         // P3 // Short L piece
        {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {0, 2}}, // P4 // Equal L piece
        {{0, 0}, {1, 0}, {2, 0}, {1, 1}, {2, 1}}, // P5 // Thumb piece
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {0, 2}}, // P6 // U piece
        {{0, 0}, {1, 0}, {2, 0}, {1, 1}, {1, 2}}, // P7 // T piece
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}},         // P8 // Lowercase Z/S piece
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}, {3, 1}}, // P9 // Lightning piece
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}}, // P10 // Uppercase Z/S piece
    };

    CellID month = CellID::Jan;
    CellID day = CellID::one;
    CellID dayOfWeek = CellID::Mon;

    if (solveBoard(board, pieces, 0, month, day, dayOfWeek))
    {
        printBoard(board);
    }
    else
    {
        cout << "No solution found." << endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    return 0;
}
