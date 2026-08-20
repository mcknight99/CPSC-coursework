#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>

#include "Game.h"

// ============================================================
// WIFI CONFIGURATION
// ============================================================
//
// Friends connect directly to:
//
//     BuckshotRoulette
//
// and then open:
//
//     192.168.4.1
//

const char* AP_SSID = "BuckshotRoulette";
const char* AP_PASSWORD = "buckshot";

// GPIO CONFIGURATION

constexpr uint8_t GPIO_TRIGGER = 4;
constexpr uint8_t GPIO_TARGET  = 5;
constexpr uint8_t GPIO_FIRE_LED = 6;


WebServer server(80);

WebSocketsServer webSocket(81);

Game game;


bool previousTriggerState = HIGH;
bool previousTargetState = HIGH;

unsigned long lastTriggerTime = 0;
unsigned long lastTargetTime = 0;

constexpr unsigned long DEBOUNCE_MS = 150;

String getContentType(const String& filename) {

    if (filename.endsWith(".html")) {
        return "text/html";
    }

    if (filename.endsWith(".css")) {
        return "text/css";
    }

    if (filename.endsWith(".js")) {
        return "application/javascript";
    }

    if (filename.endsWith(".json")) {
        return "application/json";
    }

    if (filename.endsWith(".png")) {
        return "image/png";
    }

    if (filename.endsWith(".jpg")) {
        return "image/jpeg";
    }

    if (filename.endsWith(".svg")) {
        return "image/svg+xml";
    }

    return "text/plain";
}


bool serveFile(String path) {

    if (path == "/") {
        path = "/index.html";
    }

    if (!LittleFS.exists(path)) {
        return false;
    }

    File file = LittleFS.open(path, "r");

    server.streamFile(
        file,
        getContentType(path)
    );

    file.close();

    return true;
}


void handleNotFound() {

    if (!serveFile(server.uri())) {

        server.send(
            404,
            "text/plain",
            "Not found"
        );
    }
}


void broadcastPublicState() {

    String json = game.getPublicJson();

    webSocket.broadcastTXT(
        "PUBLIC:" + json
    );
}


void sendPrivateState(uint8_t clientNum, uint8_t playerId) {

    String json = game.getPrivateJson(playerId);

    webSocket.sendTXT(
        clientNum,
        "PRIVATE:" + json
    );
}

void webSocketEvent(
    uint8_t clientNum,
    WStype_t type,
    uint8_t* payload,
    size_t length
) {

    if (type != WStype_TEXT) {
        return;
    }

    String message = String(
        reinterpret_cast<char*>(payload)
    );

    Serial.printf(
        "WS[%d] -> %s\n",
        clientNum,
        message.c_str()
    );


    if (message.startsWith("PLAYER:")) {

        int firstColon = message.indexOf(':');
        int secondColon = message.indexOf(':', firstColon + 1);

        if (secondColon > 0) {

            uint8_t playerId =
                message.substring(
                    firstColon + 1,
                    secondColon
                ).toInt();

            String name =
                message.substring(secondColon + 1);

            if (game.addPlayer(playerId, name)) {

                webSocket.sendTXT(
                    clientNum,
                    "JOINED"
                );

                sendPrivateState(
                    clientNum,
                    playerId
                );

                broadcastPublicState();

            } else {

                webSocket.sendTXT(
                    clientNum,
                    "ERROR:Could not join"
                );
            }
        }

        return;
    }


    if (message == "SPECTATOR") {

        webSocket.sendTXT(
            clientNum,
            "SPECTATOR_OK"
        );

        broadcastPublicState();

        return;
    }


    if (message == "START") {

        game.startGame();

        broadcastPublicState();

        return;
    }


    if (message == "TARGET_NEXT") {

        game.selectNextTarget();

        broadcastPublicState();

        return;
    }


    if (message == "FIRE") {

        Shell result = game.fire();

        digitalWrite(
            GPIO_FIRE_LED,
            HIGH
        );

        delay(100);

        digitalWrite(
            GPIO_FIRE_LED,
            LOW
        );

        Serial.printf(
            "Shot result: %s\n",
            result == Shell::LIVE
                ? "LIVE"
                : "BLANK"
        );

        broadcastPublicState();

        return;
    }


    // Example:
    //     ITEM:2:1
    // Player 2 uses inventory slot 1.

    if (message.startsWith("ITEM:")) {

        int colon1 = message.indexOf(':');
        int colon2 = message.indexOf(':', colon1 + 1);

        if (colon2 > 0) {

            uint8_t playerId =
                message.substring(
                    colon1 + 1,
                    colon2
                ).toInt();

            uint8_t itemIndex =
                message.substring(
                    colon2 + 1
                ).toInt();

            bool success =
                game.useItem(
                    playerId,
                    game.getPlayer(playerId)->items[itemIndex]
                );

            if (success) {

                // Refresh everyone.
                broadcastPublicState();

                // and refresh the private player state.
                
                // for implementation keep a table mapping WebSocket clients to player IDs.

            }
        }

        return;
    }
}


void processPhysicalControls() {

    unsigned long now = millis();

    bool triggerState =
        digitalRead(GPIO_TRIGGER);

    bool targetState =
        digitalRead(GPIO_TARGET);


        // target  button
    if (
        targetState == LOW &&
        previousTargetState == HIGH &&
        now - lastTargetTime > DEBOUNCE_MS
    ) {

        lastTargetTime = now;

        game.selectNextTarget();

        broadcastPublicState();

        Serial.println(
            "Physical target button pressed."
        );
    }

    // trigger logic
    if (
        triggerState == LOW &&
        previousTriggerState == HIGH &&
        now - lastTriggerTime > DEBOUNCE_MS
    ) {

        lastTriggerTime = now;

        Shell result = game.fire();

        // physical fire
        digitalWrite(
            GPIO_FIRE_LED,
            HIGH
        );

        delay(100);

        digitalWrite(
            GPIO_FIRE_LED,
            LOW
        );

        Serial.printf(
            "Physical shot: %s\n",
            result == Shell::LIVE
                ? "LIVE"
                : "BLANK"
        );

        broadcastPublicState();
    }


    previousTriggerState = triggerState;
    previousTargetState = targetState;
}

void setup() {

    Serial.begin(115200);

    delay(500);

    Serial.println();
    Serial.println(
        "=============================="
    );
    Serial.println(
        " Buckshot Roulette Server"
    );
    Serial.println(
        "=============================="
    );


    pinMode(
        GPIO_TRIGGER,
        INPUT_PULLUP
    );

    pinMode(
        GPIO_TARGET,
        INPUT_PULLUP
    );

    pinMode(
        GPIO_FIRE_LED,
        OUTPUT
    );

    digitalWrite(
        GPIO_FIRE_LED,
        LOW
    );


    if (!LittleFS.begin(true)) {

        Serial.println(
            "LittleFS mount failed!"
        );

        return;
    }

    Serial.println(
        "LittleFS mounted."
    );



    randomSeed(
        esp_random()
    );


    WiFi.mode(
        WIFI_AP
    );

    WiFi.softAP(
        AP_SSID,
        AP_PASSWORD
    );

    Serial.println(
        "WiFi AP started."
    );

    Serial.print(
        "Game address: http://"
    );

    Serial.println(
        WiFi.softAPIP()
    );

    // game.addPlayer(
    //     0,
    //     "Player 1"
    // );

    // game.addPlayer(
    //     1,
    //     "Player 2"
    // );

    // example shell configuration
    //
    // eventually this should be controlled by the round/deck
    // generation logic rather than hard-coded here

    game.loadShells(
        3,  // live
        5   // blank
    );


    server.onNotFound(
        handleNotFound
    );

    server.begin();

    Serial.println(
        "HTTP server started."
    );


    webSocket.begin();

    webSocket.onEvent(
        webSocketEvent
    );

    Serial.println(
        "WebSocket server started."
    );
}

void loop() {

    server.handleClient();

    webSocket.loop();

    processPhysicalControls();
}