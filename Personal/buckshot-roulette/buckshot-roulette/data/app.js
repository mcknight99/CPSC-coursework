let socket = null;

let myPlayerId = null;


function connectSocket() {

    /*
     * location.hostname automatically becomes the ESP32's IP.
     *
     * This means the same HTML works whether you're accessing:
     *
     *     http://192.168.4.1
     *
     * or another host later.
     */

    socket = new WebSocket(
        `ws://${location.hostname}:81/`
    );


    socket.onopen = () => {

        console.log("Connected to ESP32");

    };


    socket.onclose = () => {

        console.log("Disconnected");

        // Automatically reconnect after 1 second.
        setTimeout(connectSocket, 1000);

    };


    socket.onmessage = (event) => {

        handleMessage(event.data);

    };
}


function joinPlayer() {

    if(!socket) {
        connectSocket();   
    }

    if(!socket) {
        console.log("ERROR connecting to socket when joining player");
    }

    const select = document.querySelector(
        'input[name="playerSelect"]:checked'
    );

    

    if(select) {
        myPlayerId = Number(select.value);
    } else {
        console.log("No radio button selected");
    }


    const name =
        `Player ${myPlayerId}`;

    socket.send(
        `PLAYER:${myPlayerId}:${name}`
    );

    document.getElementById(
        "playerPanel"
    ).hidden = false;

    document.getElementById("playerSelect").hidden = true;
    document.getElementById("joinButton").hidden = true;
}


function connectSpectator() {

    connectSocket();

    const wait = setInterval(() => {

        if (
            socket &&
            socket.readyState === WebSocket.OPEN
        ) {

            socket.send("SPECTATOR");

            clearInterval(wait);
        }

    }, 100);
}


function handleMessage(message) {

    if (message.startsWith("PUBLIC:")) {

        const json =
            JSON.parse(
                message.substring(7)
            );

        updatePublicState(json);

        return;
    }


    if (message.startsWith("PRIVATE:")) {

        const json =
            JSON.parse(
                message.substring(8)
            );

        updatePrivateState(json);

        return;
    }
}


function updatePublicState(state) {

    const players =
        document.getElementById("players");


    if (!players) {
        return;
    }

    players.innerHTML = "";


    state.players.forEach(player => {

        const card =
            document.createElement("div");

        card.className = "player-card";


        if (
            player.id === state.currentPlayer
        ) {

            card.classList.add("current-turn");

        }


        if (!player.alive) {

            card.classList.add("dead");

        }


        card.innerHTML = `

            <h2>
                ${player.name}
            </h2>

            <p>
                ❤️ ${player.health}
            </p>

            <p>
                ${player.alive ? "ALIVE" : "DEAD"}
            </p>

            ${
                player.id === state.selectedTarget
                    ? "<strong>TARGET</strong>"
                    : ""
            }

        `;

        players.appendChild(card);

    });


    const status =
        document.getElementById(
            "gameStatus"
        );

    if (status) {

        status.innerHTML = `

            <p>
                Current player:
                ${state.currentPlayer + 1}
            </p>

            <p>
                Target:
                ${state.selectedTarget + 1}
            </p>

            <p>
                Shells remaining:
                ${state.shellsRemaining}
            </p>

        `;
    }
}


function updatePrivateState(state) {

    const name =
        document.getElementById("playerName");

    if (name) {
        name.textContent = state.name;
    }


    const health =
        document.getElementById("health");

    if (health) {
        health.textContent =
            state.health;
    }


    const items =
        document.getElementById("items");

    if (items) {

        items.innerHTML = "";


        state.items.forEach(
            (item, index) => {

                const button =
                    document.createElement(
                        "button"
                    );

                button.textContent =
                    item;

                button.onclick =
                    () => useItem(index);

                items.appendChild(button);
            }
        );
    }


    const secret =
        document.getElementById("secret");

    if (secret) {

        if (
            state.secret &&
            state.secret.currentShell
        ) {

            secret.textContent =
                `Current shell: ${
                    state.secret.currentShell
                }`;

        } else {

            secret.textContent =
                "No secret information.";
        }
    }
}


function useItem(index) {

    if (
        socket &&
        socket.readyState === WebSocket.OPEN
    ) {

        socket.send(
            `ITEM:${myPlayerId}:${index}`
        );
    }
}