#include "interface.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;

Interface::Interface() : gameBoard(nullptr) {}

void Interface::start()
{
    std::cout << "Load previous game or start new game? (load/new): ";
    std::string choice;
    std::cin >> choice;

    if (choice == "load" || choice == "l")
    {
        if (!loadGame())
        {
            std::cerr << "Failed to load game. Starting a new game.\n";
            newGame();
        }
    }
    else
    {
        newGame();
    }
}

bool Interface::loadGame()
{
    std::cout << "Enter log file name to load: ";
    std::string filename;
    std::cin >> filename;

    std::ifstream logFile("logs/" + filename);
    if (!logFile)
    {
        std::cerr << "Error: Log file not found.\n";
        return false;
    }

    std::vector<std::vector<char>> boardData;
    std::unordered_map<std::string, std::string> players;

    std::string line;
    bool readingBoard = true;
    bool readingPlayers = false;
    bool readingClues = false;

    while (std::getline(logFile, line))
    {
        if (line.empty())
            continue;
        if (line == "PLAYERS")
        {
            if(boardData.empty()) {
                std::cerr << "Error: Board data not found.\n";
                return false;
            }
            gameBoard = new Board(boardData);
            readingBoard = false;
            readingPlayers = true;
            std::cerr<<"Successfully read board data\n";
            gameBoard->printBoard();
            continue;
        }
        if (line == "CLUES")
        {
            if(players.empty()) {
                std::cerr << "Error: Players not found.\n";
                return false;
            }
            for (const auto &p : players)
            {
                gameBoard->addPlayer(p.first, p.second);
            }
            readingPlayers = false;
            readingClues = true;
            std::cerr<<"Successfully read players\n";
            for(const auto &p : players) {
                gameBoard->printPlayer(p.first);
            }
            continue;
        }

        if (readingBoard)
        {
            boardData.push_back(std::vector<char>(line.begin(), line.end()));
        }
        else if (readingPlayers)
        {
            size_t space = line.find(' ');
            if (space != std::string::npos)
            {
                std::string name = line.substr(0, space);
                std::string color = line.substr(space + 1);
                players[name] = color;
            }
        }
        else if (readingClues)
        {
            std::cout<<"Processing logged command: "<<line<<std::endl;
            processCommand(line);
        }
    }

    logFile.close();

    logFileName = "logs/" + filename;

    gameLoop();

    return true;
}

void Interface::newGame()
{
    enterBoardLayout();
    addPlayers();

    // Create log file
    if (!fs::exists("logs"))
        fs::create_directory("logs");

    // Generate filename based on date-time
    auto now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", std::localtime(&now));
    logFileName = "logs/" + std::string(buf) + ".txt";

    std::ofstream logFile(logFileName);
    for (const auto &row : gameBoard->originalBoard)
    {
        for (char cell : row)
            logFile << cell;
        logFile << '\n';
    }
    logFile << "PLAYERS\n";
    for (const auto &p : gameBoard->playersByName)
    {
        logFile << p.first << " " << p.second.color << '\n';
    }
    logFile << "CLUES\n";
    logFile.close();

    gameLoop();
}

void Interface::enterBoardLayout()
{
    std::vector<std::vector<char>> boardData;
    std::cout << "Enter board layout row by row ('done' to finish):\n";
    std::string row;
    while (true)
    {
        std::getline(std::cin, row);
        if (row == "done")
            break;
        boardData.push_back(std::vector<char>(row.begin(), row.end()));
    }
    gameBoard = new Board(boardData);
}

void Interface::addPlayers()
{
    std::ofstream logFile(logFileName, std::ios::app);
    std::cout << "Enter players (name color), 'done' to finish:\n";
    std::string name, color;
    while (true)
    {
        std::cin >> name;
        if (name == "done")
            break;
        std::cin >> color;
        gameBoard->addPlayer(name, color);
        logFile << name << " " << color << '\n';
    }
    logFile.close();
}

void Interface::gameLoop()
{
    bool exit = false;
    std::string command;

    while (!exit)
    {
        std::cout << "\n--------------------------------\n";
        std::cout << "Archipelago solver running...\n";
        std::cout << "Options: \n";
        std::cout << "hint <player> <island> <radius> <yes/no>\n";
        std::cout << "dig <player> <island>\n";
        std::cout << "print <player/default: board>\n";
        std::cout << "list\n";
        std::cout << "exit\n";
        std::cout << "> ";

        std::getline(std::cin >> std::ws, command);  // Trim leading whitespace and read full line
        
        if (command == "exit" || command == "e")
        {
            exit = true;
        }

        processCommand(command);
    }
}

void Interface::logMove(const std::string &move)
{
    std::ofstream logFile(logFileName, std::ios::app);
    if (logFile)
    {
        logFile << move << '\n';
    }
}

void Interface::processCommand(const std::string &command) {

    std::istringstream stream(command);
    std::string cmd, player, island, result;
    int radius;

    stream >> cmd; // Read the main command

    if(cmd == "exit" || cmd == "e") {
        std::cout<<"Thank you for using Archipelago solver, have a good day!\n";
        std::cout<<"Exiting...\n";
        return;
    }

    if (cmd == "hint" || cmd == "h") {
        if (stream >> player >> island >> radius >> result) {
            bool success = (result == "true" || result == "t" || result == "1" || result == "yes" || result == "y"); 
            if (result == "false" || result == "f" || result == "0" || result == "no" || result == "n") {
                success = false;
            } else {
                std::cerr << "Invalid hint command format.\n";
            }
            gameBoard->doHint(player, island[0], radius, success);
            logMove(cmd + " " + player + " " + island + " " + std::to_string(radius) + " " + result);
            gameBoard->printPlayerBoard(player);
        } else {
            std::cerr << "Invalid hint command format.\n";
        }
    } 
    else if (cmd == "dig" || cmd == "d") {
        if (stream >> player >> island) {
            gameBoard->doDig(player, island[0]);
            logMove(cmd + " " + player + " " + island);
            gameBoard->printPlayerBoard(player);
        } else {
            std::cerr << "Invalid dig command format.\n";
        }
    } 
    else if (cmd == "print" || cmd == "p") {
        if (stream >> player) {
            if(player == "board") {
                gameBoard->printBoard();
            } else {
                gameBoard->printPlayerBoard(player);
            }
        } else {
            gameBoard->printBoard();
        }
    } else if (cmd == "list" || cmd == "l") {
        // print all players
        for (const auto &p : gameBoard->playersByName) {
            gameBoard->printPlayer(p.first);
        }
    }
    else {
        std::cerr << "Unknown command: " << cmd << '\n';
    }
}
