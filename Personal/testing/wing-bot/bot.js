
// Wing Night Bot

//
// A Slack bot that:
//
//   /wings
//       Creates a Wing Night scoreboard.
//
//   Reply "17" to a scoreboard thread
//       Adds 17 wings to your lifetime total.
//
//   /wings stats
//       Shows your personal all-time total.
//
//   /wings leaderboard
//       Shows the top 5.
//
//   /wings help
//       Shows help.
//
// Admin commands:
//
//   /wings add @user 10
//   /wings subtract @user 10
//   /wings set @user 100
//   /wings reset @user
//


require("dotenv").config();

const { App } = require("@slack/bolt");
const Database = require("better-sqlite3");



// Configuration


const PORT = process.env.PORT || 3000;

const ADMIN_USER_IDS = new Set(
    (process.env.ADMIN_USER_IDS || "")
        .split(",")
        .map((id) => id.trim())
        .filter(Boolean)
);



// Database

//
// SQLite is a great fit for a small team.
//
// The database lives in:
//
//     wingbot.db
//
// SQLite means the data survives bot restarts.
//


const db = new Database("wingbot.db");

// Enable WAL mode.
//
// This improves SQLite's behavior when the bot is reading and
// writing at the same time.
db.pragma("journal_mode = WAL");


// Users contain lifetime totals.
db.exec(`
  CREATE TABLE IF NOT EXISTS users (
    slack_user_id TEXT PRIMARY KEY,
    display_name TEXT,
    total_wings INTEGER NOT NULL DEFAULT 0,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
  );
`);


// Each /wings invocation creates a "wing night" thread.
db.exec(`
  CREATE TABLE IF NOT EXISTS wing_nights (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    channel_id TEXT NOT NULL,
    message_ts TEXT NOT NULL UNIQUE,
    created_by TEXT NOT NULL,

    -- "open" means this is the currently active Wing Night.
    -- "closed" means the thread is stale and cannot accept
    -- additional submissions.
    status TEXT NOT NULL DEFAULT 'open',

    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    closed_at TEXT
  );
`);


// Every submission is recorded.
//
// This gives us an audit trail and, importantly, lets us prevent
// Slack from accidentally causing the same event to be counted twice.
db.exec(`
  CREATE TABLE IF NOT EXISTS submissions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    event_id TEXT NOT NULL UNIQUE,
    wing_night_id INTEGER NOT NULL,
    slack_user_id TEXT NOT NULL,
    wings INTEGER NOT NULL,
    message_ts TEXT NOT NULL,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (wing_night_id)
      REFERENCES wing_nights(id)
  );
`);



// Prepared SQL statements


const getUserStatement = db.prepare(`
  SELECT *
  FROM users
  WHERE slack_user_id = ?
`);

const createUserStatement = db.prepare(`
  INSERT OR IGNORE INTO users (
    slack_user_id,
    display_name,
    total_wings
  )
  VALUES (?, ?, 0)
`);

const updateUserTotalStatement = db.prepare(`
  UPDATE users
  SET
    total_wings = ?,
    updated_at = CURRENT_TIMESTAMP
  WHERE slack_user_id = ?
`);

const getTopUsersStatement = db.prepare(`
  SELECT
    slack_user_id,
    display_name,
    total_wings
  FROM users
  WHERE total_wings > 0
  ORDER BY total_wings DESC, slack_user_id ASC
  LIMIT 5
`);

// Find the currently active Wing Night in a channel.
//
// There should normally only be one open Wing Night per channel.
const getOpenWingNightStatement = db.prepare(`
  SELECT *
  FROM wing_nights
  WHERE channel_id = ?
    AND status = 'open'
  ORDER BY created_at DESC
  LIMIT 1
`);

// Mark a Wing Night as closed.
const closeWingNightStatement = db.prepare(`
  UPDATE wing_nights
  SET
    status = 'closed',
    closed_at = CURRENT_TIMESTAMP
  WHERE id = ?
`);

// Find a specific Wing Night by its Slack thread.
// We use this when somebody replies to a thread.
const getWingNightStatement = db.prepare(`
  SELECT *
  FROM wing_nights
  WHERE channel_id = ?
    AND message_ts = ?
`);

const createWingNightStatement = db.prepare(`
  INSERT INTO wing_nights (
    channel_id,
    message_ts,
    created_by
  )
  VALUES (?, ?, ?)
`);

const submissionExistsStatement = db.prepare(`
  SELECT id
  FROM submissions
  WHERE event_id = ?
`);

const createSubmissionStatement = db.prepare(`
  INSERT INTO submissions (
    event_id,
    wing_night_id,
    slack_user_id,
    wings,
    message_ts
  )
  VALUES (?, ?, ?, ?, ?)
`);

const getAllUsersStatement = db.prepare(`
  SELECT
    slack_user_id,
    display_name,
    total_wings
  FROM users
  ORDER BY total_wings DESC, slack_user_id ASC
`);

const getLatestWingNightStatement = db.prepare(`
  SELECT *
  FROM wing_nights
  ORDER BY id DESC
  LIMIT 1
`);


// Helper: ensure a user exists


function ensureUser(userId, displayName = null) {
    createUserStatement.run(userId, displayName);

    // If we learned the person's current display name, keep it updated.
    if (displayName) {
        db.prepare(`
      UPDATE users
      SET display_name = ?
      WHERE slack_user_id = ?
    `).run(displayName, userId);
    }
}



// Helper: get a user's score


function getUser(userId) {
    return getUserStatement.get(userId);
}



// Helper: change a user's score

//
// We use a transaction so that:
//
//   1. The user's total is updated.
//   2. The submission is recorded.
//
// happen atomically.
//


const addWingsTransaction = db.transaction(
    ({
        eventId,
        wingNightId,
        userId,
        displayName,
        wings,
        messageTs,
    }) => {
        // Make sure the user exists.
        ensureUser(userId, displayName);

        // Check whether this event was already processed.
        //
        // Slack can retry events, so this is extremely important.
        if (submissionExistsStatement.get(eventId)) {
            return {
                duplicate: true,
                user: getUser(userId),
            };
        }

        const user = getUser(userId);

        const newTotal = user.total_wings + wings;

        updateUserTotalStatement.run(
            newTotal,
            userId
        );

        createSubmissionStatement.run(
            eventId,
            wingNightId,
            userId,
            wings,
            messageTs
        );

        return {
            duplicate: false,
            user: getUser(userId),
        };
    }
);



// Slack app


const app = new App({
    token: process.env.SLACK_BOT_TOKEN,

    // Socket Mode means we don't need a public HTTP server.
    socketMode: true,

    appToken: process.env.SLACK_APP_TOKEN,
});



// Helper: format leaderboard


function leaderboardText() {
    const users = getTopUsersStatement.all();

    if (users.length === 0) {
        return "No wings have been recorded yet. Be the first! 🍗";
    }

    const medals = ["🥇", "🥈", "🥉", "4️⃣", "5️⃣"];

    return users
        .map((user, index) => {
            // Slack automatically turns <@USER_ID> into the
            // person's current display name.
            const name = `<@${user.slack_user_id}>`;

            return `${medals[index]} ${name} — *${user.total_wings} wings*`;
        })
        .join("\n");
}



// Helper: create scoreboard blocks


function scoreboardBlocks() {
    return [
        {
            type: "header",
            text: {
                type: "plain_text",
                text: "🍗 Wing Night Scoreboard",
            },
        },

        {
            type: "section",
            text: {
                type: "mrkdwn",
                text:
                    "*All-Time Top 5*\n\n" +
                    leaderboardText(),
            },
        },

        {
            type: "divider",
        },

        {
            type: "section",
            text: {
                type: "mrkdwn",
                text:
                    "🍗 *Had wings tonight?*\n" +
                    "Reply to this message in the thread with the number you had.\n\n" +
                    "_Example: `17`_",
            },
        },
    ];
}


// Closed Wing Night message

//
// When /wings is run again, we edit the old scoreboard rather
// than posting a separate "this is closed" message.
//
// This keeps the Slack channel much cleaner.


function closedScoreboardBlocks() {
    return [
        {
            type: "section",

            text: {
                type: "mrkdwn",

                text:
                    "🔒 *This Wing Night is closed.*\n\n" +
                    "This is a stale Wing Night thread and is no longer " +
                    "accepting submissions.\n\n" +
                    "Run `/wings` to start a new Wing Night.",
            },
        },
    ];
}



// /wings command

app.command("/wings-ping", async ({ ack, respond }) => {
    // Acknowledge the slash command immediately.
    await ack();

    // Send a simple response after Slack knows we received it.
    await respond("🍗 Wing Bot is alive!");
});

app.command("/wings", async ({ command, ack, client, respond }) => {
    // Slack requires slash commands to be acknowledged.
    await ack();

    const input = (command.text || "").trim();

    try {

        // /wings help


        if (input === "help") {
            await respond({
                response_type: "ephemeral",
                text:
                    "*Wing Bot commands*\n\n" +
                    "`/wings` — Start a Wing Night\n" +
                    "`/wings stats` — Show your all-time score\n" +
                    "`/wings leaderboard` — Show the top 5\n" +
                    "`/wings help` — Show this help\n\n" +
                    "*To submit wings:*\n" +
                    "Reply to a Wing Night scoreboard with a number, " +
                    "for example `17`.",
            });

            return;
        }



        // /wings stats


        if (input === "stats") {
            const user = getUser(command.user_id);

            const total = user ? user.total_wings : 0;

            await respond({
                response_type: "ephemeral",
                text:
                    `🍗 You have eaten *${total} wings* all-time.`,
            });

            return;
        }



        // /wings leaderboard


        if (input === "leaderboard") {
            await respond({
                response_type: "in_channel",
                blocks: [
                    {
                        type: "section",
                        text: {
                            type: "mrkdwn",
                            text:
                                "*🍗 All-Time Wing Leaderboard*\n\n" +
                                leaderboardText(),
                        },
                    },
                ],
            });

            return;
        }



        // Admin commands

        //
        // These are deliberately restricted to ADMIN_USER_IDS.
        //
        // Examples:
        //
        //   /wings add <@U123> 10
        //   /wings subtract <@U123> 5
        //   /wings set <@U123> 100
        //   /wings reset <@U123>
        //


        if (
            input.startsWith("add ") ||
            input.startsWith("subtract ") ||
            input.startsWith("set ") ||
            input.startsWith("reset ")
        ) {
            if (!ADMIN_USER_IDS.has(command.user_id)) {
                await respond({
                    response_type: "ephemeral",
                    text: "⛔ You aren't authorized to use admin commands.",
                });

                return;
            }

            const parts = input.split(/\s+/);

            const action = parts[0];

            const targetMatch = parts[1]?.match(/^<@([A-Z0-9]+)(?:\|[^>]+)?>$/);

            if (!targetMatch) {
                await respond({
                    response_type: "ephemeral",
                    text:
                        "Usage:\n" +
                        "`/wings add @user 10`\n" +
                        "`/wings subtract @user 10`\n" +
                        "`/wings set @user 100`\n" +
                        "`/wings reset @user`",
                });

                return;
            }

            const targetUserId = targetMatch[1];

            // Reset is special because it doesn't need a number.
            if (action === "reset") {
                ensureUser(targetUserId);

                updateUserTotalStatement.run(
                    0,
                    targetUserId
                );

                await respond({
                    response_type: "ephemeral",
                    text:
                        `🔄 <@${targetUserId}>'s score has been reset to *0 wings*.`,
                });

                return;
            }

            const amount = Number(parts[2]);

            if (!Number.isInteger(amount) || amount < 0) {
                await respond({
                    response_type: "ephemeral",
                    text: "The wing amount must be a non-negative whole number.",
                });

                return;
            }

            ensureUser(targetUserId);

            const targetUser = getUser(targetUserId);

            let newTotal;

            if (action === "add") {
                newTotal = targetUser.total_wings + amount;
            } else if (action === "subtract") {
                newTotal = Math.max(
                    0,
                    targetUser.total_wings - amount
                );
            } else if (action === "set") {
                newTotal = amount;
            }

            updateUserTotalStatement.run(
                newTotal,
                targetUserId
            );

            await respond({
                response_type: "ephemeral",
                text:
                    `🍗 <@${targetUserId}>'s new all-time score is *${newTotal} wings*.`,
            });

            return;
        }



        // Default: create a new Wing Night

        //
        // Before creating a new Wing Night, find the previous active
        // Wing Night in this channel.
        //
        // We then:
        //
        //   1. Edit the old scoreboard to say it is locked/stale.
        //   2. Mark it "closed" in the database.
        //   3. Create the new scoreboard.
        //   4. Store the new scoreboard as the active Wing Night.
        //
        // This means there is only ONE active Wing Night thread per
        // channel at any given time.




        // Step 1: Find the previous active Wing Night


        const previousWingNight =
            getOpenWingNightStatement.get(command.channel_id);



        // Step 2: Close the previous Wing Night


        if (previousWingNight) {

            try {

                // Edit the original scoreboard message.
                //
                // Slack's chat.update API allows the bot to update messages
                // that were originally posted by that bot. This replaces
                // the active scoreboard with our "closed" notice.
                await client.chat.update({
                    channel: previousWingNight.channel_id,

                    ts: previousWingNight.message_ts,

                    text:
                        "🔒 This Wing Night is closed. " +
                        "This is a stale Wing Night thread and is no longer " +
                        "accepting submissions.",

                    blocks: closedScoreboardBlocks(),
                });


            } catch (error) {

                // We don't want a failure to edit the Slack message to
                // prevent the database from being updated.
                //
                // The database is the source of truth for whether a thread
                // is active.
                console.error(
                    "Unable to update old Wing Night message:",
                    error
                );
            }


            // Mark the old Wing Night as closed.
            closeWingNightStatement.run(
                previousWingNight.id
            );
        }



        // Step 3: Create the new scoreboard


        const result = await client.chat.postMessage({

            channel: command.channel_id,

            text:
                "🍗 Wing Night Scoreboard\n\n" +
                leaderboardText(),

            blocks: scoreboardBlocks(),
        });



        // Step 4: Store the new scoreboard

        //
        // result.ts is Slack's unique timestamp for the new parent
        // message. We use it as the thread identifier when somebody
        // replies later.


        createWingNightStatement.run(
            command.channel_id,
            result.ts,
            command.user_id
        );

    } catch (error) {
        console.error("Error handling /wings:", error);

        await respond({
            response_type: "ephemeral",
            text:
                "⚠️ Something went wrong while creating the scoreboard.",
        });
    }
});



// Message handler

//
// Every message in a channel where the bot has the appropriate
// access can reach this handler.
//
// We only care about:
//
//     message.thread_ts
//
// because a Wing submission must be a reply to the scoreboard.
//


app.message(async ({ event, client }) => {
    try {
        // Ignore messages that aren't normal user messages.
        //
        // This prevents edits, deletes, bot messages, etc. from being
        // interpreted as wing submissions.
        if (event.subtype) {
            return;
        }

        // Ignore messages without a user.
        if (!event.user) {
            return;
        }

        // A Wing submission MUST be a thread reply.
        if (!event.thread_ts) {
            return;
        }

        // Look up the parent message.
        //
        // The parent message timestamp is event.thread_ts.
        const wingNight = getWingNightStatement.get(
            event.channel,
            event.thread_ts
        );



        // Ignore threads that aren't Wing Night threads.


        if (!wingNight) {
            return;
        }



        // Ignore CLOSED Wing Night threads.
        //
        // Even if somebody replies to an old thread, we don't process
        // the message and we don't add anything to their score.


        if (wingNight.status !== "open") {
            return;
        }



        // Parse the submitted number


        const text = (event.text || "").trim();

        // We intentionally accept ONLY a plain integer.
        //
        // Accepted:
        //
        //     17
        //     42
        //     100
        //
        // Rejected:
        //
        //     I had 17
        //     17 wings
        //     ~17
        //     17.5
        //
        const match = text.match(/^(\d+)$/);

        if (!match) {
            await client.chat.postMessage({
                channel: event.channel,
                thread_ts: event.thread_ts,

                text:
                    `Hey <@${event.user}>! 🍗 ` +
                    "Reply with just the number of wings you had tonight.\n\n" +
                    "_Example: `17`_",
            });

            return;
        }


        const wings = Number(match[1]);



        // Validate amount


        if (!Number.isSafeInteger(wings) || wings <= 0) {
            await client.chat.postMessage({
                channel: event.channel,
                thread_ts: event.thread_ts,

                text:
                    `Hey <@${event.user}>! Please enter a positive whole number.`,
            });

            return;
        }



        // Add the wings


        const result = addWingsTransaction({
            // A Slack message's channel + timestamp uniquely identify
            // the message we are processing.
            //
            // This also protects us if Slack retries the same event.
            eventId: `${event.channel}:${event.ts}`,

            wingNightId: wingNight.id,

            userId: event.user,

            // We no longer need users.info or users:read.
            displayName: null,

            wings,

            messageTs: event.ts,
        });

        // Slack may retry an event.
        //
        // If this event was already processed, do not add the wings
        // again.
        if (result.duplicate) {
            console.log(
                `Ignoring duplicate Slack event ${event.event_id}`
            );

            return;
        }



        // Reply with new total


        await client.chat.postMessage({
            channel: event.channel,

            thread_ts: event.thread_ts,

            text:
                `🍗 <@${event.user}> added *${wings} wings*! \n\n` +
                `Your all-time total is now *${result.user.total_wings} wings*.`,
        });


        console.log(
            `<@${event.user}> added ${wings} wings. ` +
            `New total: ${result.user.total_wings}`
        );


    } catch (error) {
        console.error(
            "Error handling Wing Night message:",
            error
        );
    }
});



// Global error logging

//
// This catches errors that escape individual event handlers,
// making debugging much easier.


app.error(async (error) => {
    console.error("Slack/Bolt error:");
    console.error(error);
});



// Startup


(async () => {
    try {
        await app.start();

        console.log("");
        console.log("========================================");
        console.log("🍗 Wing Bot is running!");
        console.log("========================================");
        console.log(
            `Admins: ${[...ADMIN_USER_IDS].join(", ") || "none"}`
        );
        console.log("");
        console.log("Waiting for Slack events...");
        console.log("");
    } catch (error) {
        console.error("❌ Failed to start Wing Bot:");
        console.error(error);

        process.exit(1);
    }
})();