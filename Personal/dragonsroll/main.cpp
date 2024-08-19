#include <iostream>
#include <vector>
#include <random>
#include <map>

class Player
{
public:
    int pipCount;
    int betAmount = 1; // ante in
    int playerIndex;
    int lastRoll;

    Player() : pipCount(0), betAmount(0), playerIndex(-1) {}

    int rollDie(int R)
    {
        std::random_device rd;                         // Seed for random number generation
        std::mt19937 gen(rd());                        // Standard mersenne_twister_engine
        std::uniform_int_distribution<> distrib(1, R); // Range [1, R]
        lastRoll = distrib(gen);
        return lastRoll;
    }

    void placeBet(double amount)
    {
        betAmount += amount;
    }

    std::string toString()
    {
        return "Player " + std::to_string(playerIndex) + "=(" + std::to_string(pipCount) + "P, " + std::to_string(betAmount) + "B)";
    }
};

class Game
{
private:
    std::vector<Player> players;
    int R;

public:
    Game(int P, int dieRange) : R(dieRange)
    {
        players.resize(P);
        for (size_t i = 0; i < players.size(); ++i)
        {
            players[i].playerIndex = i + 1;
        }
    }

    void playRound()
    {
        std::pair<int, int> highRollCount = {0, 0};

        for (auto &player : players)
        {
            int rolled = player.rollDie(R);
            player.pipCount += rolled;
            std::cout << "Player " << player.playerIndex << " rolled a " << rolled << " and now has " << player.pipCount << " pips." << std::endl;
            if (rolled > highRollCount.first)
            {
                highRollCount.first = rolled;
                highRollCount.second = 1;
            }
            else if (rolled == highRollCount.first)
            {
                highRollCount.second++;
            }
        }

        takeBets(highRollCount.first);

        if (highRollCount.second > 1)
        {
            std::cout << "There was a tie for the highest roll! Another round must be played." << std::endl;
            playRound();
        }
    }

    void takeBets(int highRoll)
    {
        for (auto &player : players)
        {
            double bet;
            std::cout << "Player " << player.playerIndex << ", place your bet. It must not be more than the highest roll (" << highRoll << "):";

            do
            {
                std::cin >> bet;
                if (bet > highRoll)
                {
                    std::cout << "Bet must not be more than the highest roll (" << highRoll << "). Try again: ";
                }
            } while (bet > highRoll);

            player.placeBet(bet);
        }
    }

    int findWinner()
    {
        int maxPips = -1;
        int winnerIndex = -1;

        for (size_t i = 0; i < players.size(); ++i)
        {
            if (players[i].pipCount > maxPips)
            {
                maxPips = players[i].pipCount;
                winnerIndex = i;
            }
        }

        return winnerIndex;
    }

    void resolveBets(int winnerIndex)
    {
        for (size_t i = 0; i < players.size(); ++i)
        {
            if (static_cast<int>(i) != winnerIndex)
            {
                double forfeiture = (players[i].betAmount*(1-(players[i].pipCount/players[winnerIndex].pipCount)));
                std::cout<< players[i].toString() << " loses " << forfeiture << " to " << players[winnerIndex].toString() << std::endl;
                
            } else {
                std::cout<< players[i].toString() << " wins!" << std::endl;
            }
        }
    }

    void startGame()
    {
        while (true)
        {
            playRound();

            int winnerIndex = findWinner();

            // Raise bets if continuing
            char continueGame;
            std::cout << "Do players want to continue betting? (y/n): ";
            std::cin >> continueGame;

            if (continueGame == 'n')
            {
                resolveBets(winnerIndex);
                break;
            }
            else
            {
                // Bets must be raised
                std::cout << "Raising bets for the next round..." << std::endl;
                takeBets(R);
            }
        }
    }
};

int main()
{
    int P = 3; // Number of players
    int R = 6; // Die range (1 to R)

    std::cout << "Enter number of players: ";
    std::cin >> P;
    std::cout << "Enter die range (R): ";
    std::cin >> R;

    Game game(P, R);
    game.startGame();

    return 0;
}
