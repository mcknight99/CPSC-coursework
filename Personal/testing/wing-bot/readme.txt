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

(i ran the app on an RPi using a linux distro) (make sure your node and npm aren't installed separately and are up to date :| )
$ sudo apt install npm 
$ git clone --filter=blob:none --sparse https://github.com/mcknight99/CPSC-coursework
$ git sparse-checkout add CPSC-coursework/Personal/testing/wing-bot
$ cd Personal/testing/wing-bot
$ npm install
$ nano .env
(fill out the .env with the above .env with correct tokens and ids)
$ npm start

(setup auto boot)
$ sudo cp directory/to/wing-bot/wing-bot.service /etc/systemd/system/wing-bot.service
(copy and fill out the correct information in wing-bot.service from this directory)
    $ sudo nano /etc/systemd/system/wing-bot.service
$ sudo systemctl daemon-reload
$ sudo systemctl start wing-bot
$ sudo systemctl status wing-bot
$ sudo systemctl enable wing-bot
    --> Created symlink ...

(check auto boot)
$ sudo systemctl kill wing-bot
OR $ sudo reboot
$ sudo systemctl status wing-bot
    --> Active: active (running)
$ systemctl is-enabled wing-bot
$ systemctl is-active wing-bot

(monitor)
$ sudo journalctl -u wing-bot -f



Start
sudo systemctl start wing-bot
Stop
sudo systemctl stop wing-bot
Restart
sudo systemctl restart wing-bot
Check status
sudo systemctl status wing-bot
Watch logs
sudo journalctl -u wing-bot -f
Make it start at boot
sudo systemctl enable wing-bot
Prevent automatic startup
sudo systemctl disable wing-bot