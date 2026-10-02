want to run this from nothing?

create an .env file in this directory, with the format:
----------------------------.env----------------------------------

SLACK_BOT_TOKEN=xoxb-...
SLACK_APP_TOKEN=xapp-1-...

# Slack user ID(s) allowed to use admin commands.
# Separate multiple IDs with commas.
ADMIN_USER_IDS=ABCDEFG1234

------------------------------------------------------------------

(windows)
> winget install OpenJS.NodeJS.LTS
(add to your path: C:\Program Files\nodejs)
> cd ..\path\to\project\wing-bot
> npm start

(i ran the app on an RPi using a linux distro)
sudo apt install npm
