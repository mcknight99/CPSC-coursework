#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <LittleFS.h>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <string>

const char *ssid = "The boys";
const char *password = "607AFA!!";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Player structure
struct Player
{
  String name;
  std::vector<int> hand;
  bool focused;
};

// Global game state
std::map<uint32_t, Player> players;
std::vector<std::pair<String, int>> playLog;
std::vector<int> deck;
std::set<uint32_t> shurikenVotes;
int roundNum = 1;
int lives = 1;
int shurikens = 1;
int lastCard = -1;
bool once = true;

// Compare helper for sorting pairs by .second
bool compareBySecond(const std::pair<String, int> &a,
                     const std::pair<String, int> &b)
{
  return a.second < b.second;
}

// Log an event by string
void logEvent(const String &evt)
{
  Serial.print("Logging event: ");
  Serial.println(evt);

  std::pair<String, int> entry;
  entry.first = evt;
  // int is the message id, 0 if there are no messages yet, otherwise increment by 1 from last id
  if (playLog.empty())
  {
    entry.second = 0;
  }
  else
  {
    entry.second = playLog.back().second + 1;
  }
  playLog.push_back(entry);
}

// Helper to format a JSON array from a vector of numbers
String jsonArray(const std::vector<int> &arr)
{
  String out = "[";
  for (size_t i = 0; i < arr.size(); ++i)
  {
    out += String(arr[i]);
    if (i + 1 < arr.size())
      out += ",";
  }
  out += "]";
  return out;
}

// Helper to format a JSON array from a vector of strings
String jsonArrayStr(const std::vector<String> &arr)
{
  String out = "[";
  for (size_t i = 0; i < arr.size(); ++i)
  {
    out += "\"" + arr[i] + "\"";
    if (i + 1 < arr.size())
      out += ",";
  }
  out += "]";
  return out;
}

// Helper to format the log
String formatLog()
{
  String out = "\"log\":[";
  // for (size_t i = 0; i < playLog.size(); ++i) {
  //   out += "\"" + playLog[i].first + "\"";
  //   if (i + 1 < playLog.size()) out += ",";
  // }
  // reverse iterate through log
  for (int i = playLog.size() - 1; i >= 0; --i)
  {
    out += "\"" + playLog[i].first + "\"";
    if (i > 0)
      out += ",";
  }
  out += "]";
  return out;
}

// Broadcast game state to each player individually (only sending their own hand)
void broadcastGameState()
{
  for (auto &pair : players)
  {
    uint32_t id = pair.first;
    Player &pl = pair.second;

    String msg = "{";

    // Core game info
    msg += "\"round\":" + String(roundNum) + ",";
    msg += "\"lives\":" + String(lives) + ",";
    msg += "\"shurikens\":" + String(shurikens) + ",";

    // Log
    msg += formatLog() + ",";

    // Players object without hands
    msg += "\"players\":{";
    size_t count = 0;
    for (auto &p : players)
    {
      msg += "\"" + String(p.first) + "\":{";
      msg += "\"name\":\"" + p.second.name + "\",";
      msg += "\"focused\":" + String(p.second.focused ? "true" : "false");
      msg += "}";
      if (++count < players.size())
        msg += ",";
    }
    msg += "},";

    // This player's hand only
    msg += "\"hand\":" + jsonArray(pl.hand) + ",";

    // Unfocused players
    std::vector<String> unfocused;
    for (auto &p : players)
    {
      if (!p.second.focused)
        unfocused.push_back(p.second.name);
    }
    msg += "\"unfocused\":" + jsonArrayStr(unfocused) + ",";

    // Shuriken vote count
    msg += "\"shurikenVotes\":" + String(shurikenVotes.size());

    msg += "}";

    ws.text(id, msg.c_str(), msg.length());
  }
  Serial.println("Game state broadcasted.");
}

// Prepare and shuffle deck
void generateDeck(int maxValue = 100)
{
  Serial.println("Generating deck...");
  deck.clear();
  for (int i = 1; i <= maxValue; ++i)
  {
    deck.push_back(i);
  }
  std::random_shuffle(deck.begin(), deck.end());
}

// Deal hands for current round
void dealHands()
{
  Serial.println("Dealing hands...");
  logEvent("Dealing hands for round " + String(roundNum));
  // if the amount of players times the round number is less than 100, generate a deck with size 100, otherwise use a deck of players times round number to properly deal cards
  generateDeck(players.size() * roundNum < 100 ? 100 : players.size() * roundNum);
  lastCard = -1;
  for (std::map<uint32_t, Player>::iterator it = players.begin(); it != players.end(); ++it)
  {
    it->second.hand.clear();
    for (int k = 0; k < roundNum; ++k)
    {
      it->second.hand.push_back(deck.back());
      deck.pop_back();
    }
    std::sort(it->second.hand.begin(),
              it->second.hand.end());
  }
}

// Check if all players have clicked focus
bool allFocused()
{
  Serial.println("Checking if all players are focused...");
  // if there are less than 2 players, return false
  if (players.size() < 2)
  {
    Serial.println("Not enough players to start the game.");
    return false;
  }

  for (std::map<uint32_t, Player>::iterator it = players.begin();
       it != players.end(); ++it)
  {
    if (!it->second.focused)
    {
      Serial.println(it->second.name + " is not focused.");
      return false;
    }
  }
  Serial.println("All players are focused.");
  return true;
}

// Reset focus flags for next round
void resetFocus()
{
  Serial.println("Resetting focus flags...");
  for (std::map<uint32_t, Player>::iterator it = players.begin();
       it != players.end(); ++it)
  {
    it->second.focused = false;
  }
}

// Award shuriken/life rewards after a won round
void updateRoundRewards()
{
  Serial.println("Updating round rewards...");
  if (roundNum % 2 == 0)
  {
    shurikens++;
    logEvent("Gained a shuriken");
  }
  if (roundNum % 3 == 0)
  {
    lives++;
    logEvent("Gained a life");
  }
}

// Reveal lowest cards for shuriken
void revealLowestCards()
{
  Serial.println("Processing shuriken...");
  logEvent("Processing shuriken...");
  // ensure no players have exactly one card in hand, shuriken may be played when a player has no cards
  for (std::map<uint32_t, Player>::iterator it = players.begin();
       it != players.end(); ++it)
  {
    if (it->second.hand.size() == 1)
    {
      logEvent(it->second.name + " does not have enough cards to play shuriken");
      return;
    }
  }

  std::vector<std::pair<String, int>> lowest;
  for (std::map<uint32_t, Player>::iterator it = players.begin();
       it != players.end(); ++it)
  {
    lowest.push_back(std::make_pair(
        it->second.name,
        it->second.hand.front()));
    // remove the lowest card from the player's hand
    it->second.hand.erase(it->second.hand.begin());
  }
  std::sort(lowest.begin(), lowest.end(), compareBySecond);
  logEvent("Shuriken played");
  for (size_t i = 0; i < lowest.size(); ++i)
  {
    logEvent(lowest[i].first + " reveals " +
             String(lowest[i].second));
  }
  
  shurikens--;
  broadcastGameState();
  Serial.println("Shuriken processed.");
}

void resetShurikenVotes()
{
  Serial.println("Resetting shuriken votes...");
  shurikenVotes.clear();
}

// Handle a PLAY command from a client
void handlePlay(uint32_t id)
{
  Serial.println("Handling play command from player " + String(id));
  // check if all players are focused
  if (!allFocused())
  {
    logEvent("Not all players are focused");
    broadcastGameState();
    return;
  }

  std::map<uint32_t, Player>::iterator it = players.find(id);
  if (it == players.end())
    return;
  Player &p = it->second;
  if (p.hand.empty())
    return;

  int card = p.hand.front();
  p.hand.erase(p.hand.begin());

  if (card <= lastCard)
  {
    lives--;
    logEvent(p.name + " plays " + String(card));
    logEvent(String(card) + " played out of order");
    resetFocus();
    if (lives <= 0)
    {
      logEvent("GAME OVER");
      broadcastGameState();
      // reset game state
      once = true;
      roundNum = 1;
      lives = players.size();
      shurikens = 1;
      for (std::map<uint32_t, Player>::iterator jt = players.begin();
           jt != players.end(); ++jt)
      {
        jt->second.hand.clear();
      }
      deck.clear();
      shurikenVotes.clear();
      lastCard = -1;
      logEvent("Game reset");
      dealHands();
      broadcastGameState();

      return;
    }
    else
    {
      logEvent("Lost a life, " + String(lives) + " lives left");
      // clear and deal new hands
      for (std::map<uint32_t, Player>::iterator jt = players.begin();
           jt != players.end(); ++jt)
      {
        jt->second.hand.clear();
      }
      dealHands();
    }
  }
  else
  {
    lastCard = card;
    int totalRemainingCards = 0;
    for (std::map<uint32_t, Player>::iterator jt = players.begin();
         jt != players.end(); ++jt)
    {
      totalRemainingCards += jt->second.hand.size();
    }
    logEvent(p.name + " plays " + String(card) + ". (" +
             String(p.hand.size()) + "H/" +
             String(totalRemainingCards) + "T)");
  }

  // Check for round win
  bool won = true;
  for (std::map<uint32_t, Player>::iterator jt = players.begin();
       jt != players.end(); ++jt)
  {
    if (!jt->second.hand.empty())
    {
      won = false;
      break;
    }
  }
  if (won)
  {
    playLog.clear();
    logEvent("Round " + String(roundNum) + " won");
    updateRoundRewards();
    roundNum++;
    dealHands();
    resetFocus();
    resetShurikenVotes();
  }
  broadcastGameState();
}

// Set up WebSocket events
void setupWebSocket()
{
  Serial.println("Setting up WebSocket...");
  ws.onEvent([](AsyncWebSocket *server,
                AsyncWebSocketClient *client,
                AwsEventType type, void *arg,
                uint8_t *data, size_t len)
             {
    uint32_t id = client->id();

    if (type == WS_EVT_CONNECT) {
      Serial.println("Client connected: " + String(id));
    }
    else if (type == WS_EVT_DISCONNECT) {
      Serial.println("Client disconnected: " + String(id));
      players.erase(id);
      shurikenVotes.erase(id);
    }
    else if (type == WS_EVT_DATA) {
      //Serial.println("Data received from client " + String(id) + ": " + String((char*)data));
      String msg = String((char*)data);
      // find the command in the message, surrounded by #
      int start = msg.indexOf('#');
      int end = msg.indexOf('#', start + 1);
      if (start != -1 && end != -1) {
        msg = msg.substring(start + 1, end);
      }
      //Serial.println("Parsed message: " + msg);

      if (msg.startsWith("NAME:")) {
        Player np; 
        np.name = "unnamed" + String(id);
        np.focused = false;
        players[id] = np; 
        String nm = msg.substring(5);
        // Validate non-blank, trim spaces
        nm.trim();
        if (nm.length() > 0) {
          players[id].name = nm;
          logEvent("Player " + nm + " joined");
          broadcastGameState();
        }
        Serial.println("Player " + String(id) + " set name to: " + nm);
        // if name is "reset" to lowercase, reset the microcontroller
        if(nm.equalsIgnoreCase("reset")) {
          Serial.println("Resetting microcontroller...");
          ESP.restart();
        }
      }
      else if (msg == "FOCUS") {
        Serial.println("Player " + players[id].name + " focused");
        players[id].focused = true;
        if (allFocused() && once) {
          playLog.clear();
          logEvent("All players focused, starting game");
          dealHands();
          once = false;
          // set lives to num players
          lives = players.size();
        }
        broadcastGameState();
      }
      else if (msg == "PLAY") {
        Serial.println("Player " + players[id].name + " played a card. (" + String(players[id].hand.size()) + " cards left)");
        handlePlay(id);
      }
      else if (msg == "VOTE_SHURIKEN") {
        if(!allFocused()) {
          Serial.println("Not all players are focused.");
          return;
        }

        Serial.println("Player " + players[id].name + " voted for shuriken");
        shurikenVotes.insert(id);
        broadcastGameState();
        if (shurikenVotes.size() == players.size() && shurikens>0) {
          revealLowestCards();
          shurikenVotes.clear();
          broadcastGameState();
        }
      }
    } });

  server.addHandler(&ws);
}

// prototype for listing files in LittleFS
void listDir(fs::FS &fs, const char *dirname, uint8_t levels = 0);

void setup()
{
  Serial.begin(115200);
  Serial.println("Connected to Serial");

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  Serial.println("Mounting LittleFS...");
  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS mount failed");
    for (int i = 0; i < 3; i++)
    {
      Serial.println("Retrying mount...");
      delay(1000);
      if (LittleFS.begin(true))
        break;
    }
    if (!LittleFS.begin(true))
    {
      Serial.println("Failed to mount LittleFS, aborting...");
      return;
    }
  }
  Serial.println("LittleFS mounted successfully");

  Serial.println("Printing LittleFS contents...");
  listDir(LittleFS, "/", 0);
  Serial.println("Done printing LittleFS contents");

  Serial.println("Setting up server...");

  server.serveStatic("/", LittleFS, "/")
      .setDefaultFile("index.html");

  setupWebSocket();
  server.begin();
  Serial.println("Server started, ready for users.");
}

void loop()
{
  // All handled by Async server
}

// Function to list files in the LittleFS recursively down levels
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
  Serial.printf("Done listing directory: %s\n", dirname);
}