#include "interface.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <algorithm>

namespace fs = std::filesystem;

Interface::Interface() : gameBoard(nullptr) {}

void Interface::start()
{
    std::cout << "Load previous game or start new game? (load/new): \n";
    std::cout<<"> ";
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
    std::cout<<"> ";
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
        if (line == "#PLAYERS")
        {
            if (boardData.empty())
            {
                std::cerr << "Error: Board data not found.\n";
                return false;
            }
            gameBoard = new Board(boardData);
            readingBoard = false;
            readingPlayers = true;
            std::cerr << "Successfully read board data\n";
            gameBoard->printBoard();
            continue;
        }
        if (line == "#CLUES")
        {
            if (players.empty())
            {
                std::cerr << "Error: Players not found.\n";
                return false;
            }
            for (const auto &p : players)
            {
                gameBoard->addPlayer(p.first, p.second);
            }
            readingPlayers = false;
            readingClues = true;
            std::cerr << "Successfully read players\n";
            for (const auto &p : players)
            {
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
            processCommand(line, false);
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
    logFileName = "logs/" + std::string(buf) + ".log";

    std::ofstream logFile(logFileName);
    for (const auto &row : gameBoard->originalBoard)
    {
        for (char cell : row)
            logFile << cell;
        logFile << '\n';
    }
    logFile << "#PLAYERS\n";
    for (const auto &p : gameBoard->playersByName)
    {
        logFile << p.first << " " << p.second->color << '\n';
    }
    logFile << "#CLUES\n";
    logFile.close();

    gameLoop();
}

void Interface::enterBoardLayout()
{
    std::vector<std::vector<char>> boardData;
    std::cout << "Enter board layout row by row ('#done' to finish):\n";
    std::string row;
    std::cin.ignore();
    while (true)
    {
        std::cout<<"> ";
        std::getline(std::cin, row);
        if (row == "#done")
            break;
        // turn each letter into uppercase
        for (char &c : row)
        {
            if (c >= 'a' && c <= 'z')
            {
                c = c - 'a' + 'A';
            }
        }
        boardData.push_back(std::vector<char>(row.begin(), row.end()));
    }
    // normalize board size to a rectangle
    size_t maxCols = 0;
    for (const auto &row : boardData)
    {
        maxCols = std::max(maxCols, row.size());
    }
    for (auto &row : boardData)
    {
        row.resize(maxCols, ' ');
    }
    // remove empty rows at top or bottom
    while (!boardData.empty() && std::all_of(boardData.front().begin(), boardData.front().end(), [](char c) { return c == ' '; }))
    {
        boardData.erase(boardData.begin());
    }
    while (!boardData.empty() && std::all_of(boardData.back().begin(), boardData.back().end(), [](char c) { return c == ' '; }))
    {
        boardData.pop_back();
    }
    // remove empty columns at left or right
    while (!boardData.empty() && std::all_of(boardData.begin(), boardData.end(), [](const std::vector<char> &row) { return row.front() == ' '; }))
    {
        for (auto &row : boardData)
        {
            row.erase(row.begin());
        }
    }
    while (!boardData.empty() && std::all_of(boardData.begin(), boardData.end(), [](const std::vector<char> &row) { return row.back() == ' '; }))
    {
        for (auto &row : boardData)
        {
            row.pop_back();
        }
    }
    // replace anything that isn't of the following with a space
    // A-Z, $
    for (auto &row : boardData)
    {
        for (char &c : row)
        {
            if (!(c >= 'A' && c <= 'Z') && c != '$')
            {
                c = ' ';
            }
        }
    }
    // not having any of these caused a very odd bug where only new games were instanced incorrectly 
    // but loaded games were fine due to how i stored and loaded boards
    // without this input validation, the player's personal boards would have an extra row at the start
    // so to fix two bugs with one stone, i added this input validation to make everything pretty
    gameBoard = new Board(boardData);
}

void Interface::addPlayers()
{
    std::ofstream logFile(logFileName, std::ios::app);
    std::cout << "Enter players (name color), '#done' to finish:\n";
    std::string name, color;
    while (true)
    {
        std::cout<<"> ";
        std::cin >> name;
        if (name == "#done")
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
        std::cout << "remove <island>\n";
        std::cout << "print <player/default: board>\n";
        std::cout << "list\n";
        std::cout << "exit\n";
        std::cout << "> ";

        if (!std::getline(std::cin >> std::ws, command))
        {
            std::cerr << "\n[ERROR] Input stream closed. Exiting program.\n";
            exit = true;
            command = "exit";
        }

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
    } else {
        std::cerr << "Failed to log move: " << move << '\n';
    }
    logFile.close();
}

void Interface::processCommand(const std::string &command, bool log)
{
    std::cout<< "Processing command: " << command << std::endl;

    std::istringstream stream(command);
    std::string cmd, player, island, result;
    int radius;

    stream >> cmd; // Read the main command

    if (cmd == "exit" || cmd == "e")
    {
        std::cout << "Thank you for using Archipelago Ultra Rich Text Editor, have a good day!\n"; 
        std::cout << "Exiting...\n";
        return;
    }

    if (cmd == "hint" || cmd == "h")
    {
        stream >> player >> island >> radius >> result;
        bool success = (result == "true" || result == "t" || result == "1" || result == "yes" || result == "y");
        if (result == "false" || result == "f" || result == "0" || result == "no" || result == "n")
        {
            success = false;
        }
        else if (!success)
        {
            std::cerr << "Invalid hint command format.\n";
            return;
        }
        gameBoard->doHint(player, island[0], radius, success);
        if (log) logMove(cmd + " " + player + " " + island + " " + std::to_string(radius) + " " + result);
        gameBoard->printPlayerBoard(player);
    }
    else if (cmd == "dig" || cmd == "d")
    {
        stream >> player >> island;
        gameBoard->doDig(player, island[0]);
        if (log) logMove(cmd + " " + player + " " + island);
        gameBoard->printPlayerBoard(player);
    }
    else if (cmd == "print" || cmd == "p")
    {
        if (stream >> player)
        {
            if (player == "board")
            {
                gameBoard->printBoard();
            }
            else
            {
                gameBoard->printPlayerBoard(player);
            }
        }
        else
        {
            gameBoard->printBoard();
        }
    }
    else if (cmd == "list" || cmd == "l")
    {
        // print all players
        for (const auto &p : gameBoard->playersByName)
        {
            gameBoard->printPlayer(p.first);
        }
    }
    else if (cmd == "remove" || cmd == "r")
    {
        // dig island on all player's boards
        stream >> island;

        for (const auto &p : gameBoard->playersByName)
        {
            gameBoard->doDig(p.first, island[0]);
        }

        if (log) logMove(cmd + " " + island);
    }
    else
    {
        std::cerr << "Unknown command: " << cmd << '\n';
    }
}
