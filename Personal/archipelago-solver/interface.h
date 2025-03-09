#ifndef INTERFACE_H
#define INTERFACE_H

#include "board.h"
#include <string>
#include <iostream>
#include <sstream>

class Interface {
public:
    Interface();
    void start();
    
private:
    Board* gameBoard;
    std::string logFileName;
    bool loadGame();

    void newGame();
    void enterBoardLayout();
    void addPlayers();
    void gameLoop();
    void logMove(const std::string& move);
    void processCommand(const std::string& command);
};

#endif
