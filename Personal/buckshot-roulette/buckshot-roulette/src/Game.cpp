#include "Game.h"

Game::Game() {
    reset();
}

void Game::reset() {
    players.clear();
    shells.clear();

    shellIndex = 0;
    playerCount = 0;

    // player index starts at 1. no player should have index 0
    currentPlayer = 0;
    selectedTarget = 0; // no target selected

    gameRunning = false;
}

void Game::startGame() {
    if (playerCount < 2) {
        Serial.println("Not enough players to start the game.");
        Serial.printf("%d players are in the game.", playerCount);
        return;
    }
    gameRunning = true;

    // pick random player to start
    currentPlayer = random(0, playerCount); // * does this work? test the random function
    selectedTarget = findNextAlivePlayer(currentPlayer);

    Serial.printf("Game started with %d players. Player %d starts.\n", playerCount, currentPlayer);
    Serial.printf("Player %d is targeting Player %d.\n", currentPlayer, selectedTarget);
}

bool Game::addPlayer(uint8_t id, const String& name) {
    if (playerCount >= MAX_PLAYERS) {
        Serial.println("Cannot add more players. Maximum number of players reached.");
        return false;
    }

    // check if player with the same id already exists
    if(getPlayer(id) != nullptr) {
        Serial.printf("Player with ID %d already exists.\n", id);
        return false;
    }

    Player newPlayer;
    newPlayer.id = id;
    newPlayer.name = name;

    players.push_back(newPlayer);
    playerCount++;

    Serial.printf("Added Player %d: %s\n", id, name.c_str());
    return true;
}

Player* Game::getPlayer(uint8_t id) {
    for(auto& player : players) {
        if(player.id == id) {
            return &player;
        }
    }
    return nullptr;
}

const Player* Game::getPlayer(uint8_t id) const {
    for(const auto& player : players) {
        if(player.id == id) {
            return &player;
        }
    }
    return nullptr;
}

uint8_t Game::findNextAlivePlayer(uint8_t startIndex) const {
    if(players.empty()) {
        Serial.println("No players in the game.");
        return 0; // No players in the game
    }

    // start one position after the supplied player. may need to support reversing order later (lowkey just reverse the list lmao)
    // loop through list using modulo of playerCount to wrap around going from currentID+1 to currentID+playerCount with modulo

    for(uint8_t i = 1; i <= playerCount; ++i) {
        uint8_t nextIndex = (startIndex + i) % playerCount;
        if(players[nextIndex].alive && !players[nextIndex].handcuffed) {
            return players[nextIndex].id;
        }
        if(players[nextIndex].alive && players[nextIndex].handcuffed) {
            Serial.printf("Player %d is handcuffed and cannot take their turn.\n", players[nextIndex].id);
            // remove handcuff effect after skipping their turn
            const_cast<Player&>(players[nextIndex]).handcuffed = false; // crazy solution does this work???
        }
    }
    Serial.println("No other alive players found. Next alive player is the current player. (may happen with handcuffs and 2 players left)");
    return startIndex; 
}

void Game::nextTurn() {
    if(!gameRunning) {
        Serial.println("Game is not running. Cannot proceed to next turn.");
        return;
    }

    currentPlayer = findNextAlivePlayer(currentPlayer);
    selectedTarget = findNextAlivePlayer(currentPlayer);

    Serial.printf("It's now Player %d's turn.\n", currentPlayer);
    Serial.printf("Player %d is targeting Player %d.\n", currentPlayer, selectedTarget);
}

void Game::selectNextTarget() {
    if(!gameRunning) {
        Serial.println("Game is not running. Cannot select next target.");
        return;
    }

    if(players.empty()) {
        Serial.println("No players in the game.");
        return;
    }

    selectedTarget = findNextAlivePlayer(selectedTarget);

    Serial.printf("Player %d is now targeting Player %d.\n", currentPlayer, selectedTarget);
}

void Game::loadShells(uint8_t live, uint8_t blanks) {
    shells.clear();
    shellIndex = 0;

    for(uint8_t i = 0; i < live; ++i) {
        shells.push_back(Shell::LIVE);
    }
    for(uint8_t i = 0; i < blanks; ++i) {
        shells.push_back(Shell::BLANK);
    }

    // shuffle the shells
    for(uint8_t i = 0; i < shells.size(); ++i) {
        uint8_t j = random(0, shells.size());
        std::swap(shells[i], shells[j]);
    }

    Serial.printf("Loaded %d live shells and %d blank shells.\n", live, blanks);
    Serial.print("Shells: ");
    for(auto shell : shells) {
        Serial.printf("%s ", shellToString(shell).c_str());
    }
    Serial.println();

    shellStartingCount = shells.size();
    Serial.printf("Total shells loaded: %d\n", shellStartingCount);
}

Shell Game::fire() {
    if(!gameRunning) {
        Serial.println("Game is not running. Cannot fire.");
        return Shell::BLANK; // or handle this case differently
    }

    if(!hasShells()) {
        Serial.println("No shells left to fire!.");
        return Shell::BLANK; // or handle this case differently
    }

    if(shellIndex >= shells.size()) {
        Serial.println("No more shells to fire.!");
        return Shell::BLANK; // or handle this case differently
    }

    Shell firedShell = shells[shellIndex];
    shellIndex++;

    Serial.printf("Player %d fired at Player %d and got a %s shell.\n", currentPlayer, selectedTarget, shellToString(firedShell).c_str());

    // Handle the effect of the shell
    if(firedShell == Shell::LIVE) {
        Player* targetPlayer = getPlayer(selectedTarget);
        if(targetPlayer) {
            targetPlayer->health = targetPlayer->health - (saw ? 2 : 1); // double damage if saw is true
            Serial.printf("Player %d's health is now %d.\n", targetPlayer->id, targetPlayer->health);
            if(targetPlayer->health <= 0) {
                targetPlayer->health = 0;
                targetPlayer->alive = false;
                Serial.printf("Player %d has been eliminated!\n", targetPlayer->id);
            }
        }
    }

    // if blank and fired at self, next player is self. otherwise just continue to next player

    if(firedShell == Shell::BLANK && currentPlayer == selectedTarget) {
        Serial.printf("Player %d fired a blank at themselves. They get another turn.\n", currentPlayer);
    } else {
        nextTurn();
    }

    saw = false; // reset saw effect after firing no matter what shell was fired

    return firedShell;
}

bool Game::useItem(uint8_t playerId, Item item) {
    Player* player = getPlayer(playerId);
    if(!player) {
        Serial.printf("Player %d not found. Cannot use item.\n", playerId);
        return false;
    }

    if(!player->alive) {
        Serial.printf("Player %d is not alive. Cannot use item.\n", playerId);
        return false;
    }

    // Check if the player has the item
    auto it = std::find(player->items.begin(), player->items.end(), item);
    if(it == player->items.end()) {
        Serial.printf("Player %d does not have the item %s.\n", playerId, itemToString(item).c_str());
        return false;
    }

    // Remove the used item from the player's inventory
    player->items.erase(it);

    // Use the item
    switch(item) {
        case Item::BEER:
            // beer effect: skip current shell and reveal to all
            Serial.printf("Player %d used BEER. Skipping current shell of type: %s\n", playerId, shellToString(shells[shellIndex]).c_str());
            shellIndex++; // skip current shell
            break;
        case Item::MAGNIFYING_GLASS:
            // magnifying glass effect (reveal next shell)
            Serial.printf("Player %d used MAGNIFYING_GLASS.\n", playerId);
            Serial.printf("\tTelling player that next shell is: %s\n", shellToString(shells[shellIndex]).c_str());
            break;
        case Item::CIGARETTES:
            // restore 1 health
            player->health += 1;
            Serial.printf("Player %d used CIGARETTES and restored 1 health. New health: %d\n", playerId, player->health);
            break;
        case Item::HANDCUFFS:
            // Implement handcuffs effect // skip targeted player's next turn // this means they must select the handcuffed player as the target on the selector then use the item on the phone
            // IMPLEMENT: UI instead of gun targeting
            // give the user the option of a list of the players and handcuff the selected player
            // Serial.printf("Player %d used HANDCUFFS on targeted player %d.\n", playerId, selectedTarget);
            // Player* handcuffedPlayer = getPlayer(selectedTarget);
            // if(handcuffedPlayer) {
            //     handcuffedPlayer->handcuffed = true;
            //     Serial.printf("Player %d is now handcuffed and cannot take their next turn.\n", selectedTarget);
            // }
            Serial.printf("implementing HANDCUFFS effect is not yet complete. Player %d used HANDCUFFS.\n", playerId);
            break;
        case Item::SAW:
            saw = true; // next live shell does double damage
            Serial.printf("Player %d used SAW. Next shell if live will do double damage. Does nothing for a blank.\n", playerId);
            break;
        case Item::INVERTER:
            // // swap current shell with opposite type (live <-> blank)
            
            Serial.printf("Player %d used INVERTER.\n", playerId);
            Serial.printf("\tCurrent shell was: %s\n", shellToString(shells[shellIndex]).c_str());
            if (shellIndex < shells.size()) {
                if (shells[shellIndex] == Shell::LIVE) {
                    shells[shellIndex] = Shell::BLANK;
                } else {
                    shells[shellIndex] = Shell::LIVE;
                }
            }
            Serial.printf("\tCurrent shell is now: %s\n", shellToString(shells[shellIndex]).c_str());
            break;
        case Item::PHONE:
            // Implement phone effect // random information given about any coming shell (pick random index in remaining shells and tell player what it is)
            Serial.printf("Player %d used PHONE.\n", playerId);
            if (shellIndex < shells.size()) {
                uint8_t randomIndex = random(shellIndex, shellStartingCount-shellIndex-1); // pick random index in remaining shells //* find out how random works still lol
                Serial.printf("\tRandom shell at index %d is: %s\n", randomIndex, shellToString(shells[randomIndex]).c_str());
            } else {
                Serial.println("\tNo remaining shells to reveal.");
            }
            break;
        case Item::ADRENALINE:
            // implement adrenaline // steal a select item from a target player (must be alive and have items)
            Serial.printf("Player %d used ADRENALINE on targeted player %d.\n", playerId, selectedTarget);
            // need to give the player the option of all players and items from their phone and get a pair <playerId, item> to steal from the target player
            break;
        default:
            Serial.printf("Unknown item used by Player %d.\n", playerId);
            return false;
    }


    return true;
}

String Game::shellToString(Shell shell) const {
    switch(shell) {
        case Shell::LIVE:
            return "LIVE";
        case Shell::BLANK:
            return "BLANK";
        default:
            return "UNKNOWN";
    }
}

String Game::itemToString(Item item) const {
    switch(item) {
        case Item::BEER:
            return "BEER";
        case Item::MAGNIFYING_GLASS:
            return "MAGNIFYING GLASS";
        case Item::CIGARETTES:
            return "CIGARETTES";
        case Item::HANDCUFFS:
            return "HANDCUFFS";
        case Item::SAW:
            return "SAW";
        case Item::INVERTER:
            return "INVERTER";
        case Item::ADRENALINE:
            return "ADRENALINE";
        case Item::PHONE:
            return "PHONE";
        default:
            return "UNKNOWN ITEM";
    }
}

String Game::getPublicJson() const {

    String json = "{";

    json += "\"running\":";
    json += gameRunning ? "true" : "false";

    json += ",\"currentPlayer\":";
    json += String(currentPlayer);

    json += ",\"selectedTarget\":";
    json += String(selectedTarget);

    json += ",\"shellsRemaining\":";
    json += String(shellsRemaining());

    json += ",\"players\":[";

    for (size_t i = 0; i < players.size(); i++) {

        const Player& p = players[i];

        if (i > 0) {
            json += ",";
        }

        json += "{";

        json += "\"id\":";
        json += String(p.id);

        json += ",\"name\":\"";
        json += p.name;
        json += "\"";

        json += ",\"health\":";
        json += String(p.health);

        json += ",\"alive\":";
        json += p.alive ? "true" : "false";

        json += "}";

    }

    json += "]}";

    return json;
}

String Game::getPrivateJson(uint8_t playerId) const {

    const Player* player = getPlayer(playerId);

    if (player == nullptr) {
        return "{}";
    }

    String json = "{";

    json += "\"id\":";
    json += String(player->id);

    json += ",\"name\":\"";
    json += player->name;
    json += "\"";

    json += ",\"health\":";
    json += String(player->health);

    json += ",\"alive\":";
    json += player->alive ? "true" : "false";

    json += ",\"items\":[";

    for (size_t i = 0; i < player->items.size(); i++) {

        if (i > 0) {
            json += ",";
        }

        json += "\"";
        json += itemToString(player->items[i]);
        json += "\"";
    }

    json += "]";


    json += ",\"secret\":{";

    if (hasShells()) {

        json += "\"currentShell\":\"";
        json += shellToString(shells[shellIndex]);
        json += "\"";

    }

    json += "}";

    json += "}";

    return json;
}