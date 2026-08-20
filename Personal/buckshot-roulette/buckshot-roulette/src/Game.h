#define GAME_H

#include <Arduino.h>
#include <vector>

enum class Shell {
    LIVE,
    BLANK
};

enum class Item {
    BEER,
    MAGNIFYING_GLASS,
    CIGARETTES,
    HANDCUFFS,
    SAW,
    INVERTER,
    PHONE,
    ADRENALINE 
};


// might delete
enum class Action {
    FIRE,
    ITEM
};

struct Player {
    uint8_t id;

    String name; 

    int health = 4; // starting health

    std::vector<Item> items;

    bool alive = true;

    bool handcuffed = false; // if true, player cannot take their next turn
};

class Game{
    public:
    static constexpr uint8_t MAX_PLAYERS = 8;

    Game();

    void reset();

    void startGame();

    bool isRunning() const { 
        return gameRunning;
    }

    bool addPlayer(uint8_t id, const String& name);

    Player* getPlayer(uint8_t id);

    const Player* getPlayer(uint8_t id) const;

    uint8_t getPlayerCount() const {
        return playerCount;
    }

    uint8_t getCurrentPlayer() const {
        return currentPlayer;
    }

    void nextTurn();

    uint8_t getSelectedTarget() const {
        return selectedTarget;
    }

    void selectNextTarget();

    void loadShells(uint8_t live, uint8_t blanks);

    Shell fire();

    bool hasShells() const {
        return shellIndex < shells.size();
    }

    uint8_t shellsRemaining() const {
        return shells.size() - shellIndex;
    }

    bool useItem(uint8_t playerId, Item item);

    String getPublicJson() const;

    String getPrivateJson(uint8_t playerId) const;

    private:
    std::vector<Player> players;

    std::vector<Shell> shells;

    uint8_t shellIndex = 0;

    uint8_t shellStartingCount = 0;

    uint8_t playerCount = 0;

    uint8_t currentPlayer = 0;

    uint8_t selectedTarget = 0;

    bool gameRunning = false;

    bool saw = false;

    uint8_t findNextAlivePlayer(uint8_t startIndex) const;

    String shellToString(Shell shell) const;

    String itemToString(Item item) const;

};