# 1. ~$ PDBot
![PD logo](https://cdn.discordapp.com/icons/668000598221651975/9c38858cf48263dbbc5ef4b3315bdc6d.webp?size=64)

PDBot is the official Discord bot for the [Programmer's Den](https://pden.net) server. It provides many utilities for the management, moderation, and members of this server

## 1.1. Table of contents
- [1. ~$ PDBot](#1--pdbot)
  - [1.1. Table of contents](#11-table-of-contents)
  - [1.2. Dependencies and install instructions](#12-dependencies-and-install-instructions)
  - [1.3. Bot config file](#13-bot-config-file)
  - [1.4. Compiling and running bot](#14-compiling-and-running-bot)
  - [1.5. Commands](#15-commands)
  - [1.6. Latest major change](#16-latest-major-change)
  - [1.7. Latest minor change](#17-latest-minor-change)
  - [1.8. To-do](#18-to-do)

## 1.2. Dependencies and install instructions
> Requirements
```bash
apt install -y git build-essential libcurl4-openssl-dev cmake libsqlite3-dev
```

> [concord](https://github.com/Cogmasters/concord)
```bash
apt install -y git build-essential libcurl4-openssl-dev libssl-dev

git clone "https://github.com/Cogmasters/concord.git"

cd concord/
make purge
make -j$(nproc) # or number of threads
make install
```

> [json-c](https://github.com/json-c/json-c)
```bash
apt install -y git cmake build-essential

git clone "https://github.com/json-c/json-c.git"

cd json-c/
mkdir build
cd build/
cmake ../
make -j$(nproc) # or number of threads
make test
make install
```
> [sqlite3](https://sqlite.org/index.html)
```bash
apt install -y libsqlite3-dev
```

## 1.3. Bot config file
> Create a `bot.config` file in the root directory of the bot with the following contents

> Delete comments if copy and pasting from below or will not work
```json
{
  "logging": {
    "level": "trace",
    "filename": "bot.log",
    "quiet": false, // change to true to disable logs in console
    "overwrite": false,
    "use_color": true,
    "http_dump": {
      "enable": true,
      "filename": "dump.json"
    }
  },
  "discord": {
    "token": "TOKEN", // place bot token here
    "default_prefix": {
      "enable": true,
      "prefix": "./" // bot prefix
    }
  }
}
```

## 1.4. Compiling and running bot
```bash
git clone "https://github.com/programmers-den/PDBot.git"

cd PDBot/
# Must create bot.config and paste token as defined in section 1.2

make -j$(nproc) # or number of threads
./main
```


## 1.5. Commands
> The prefix is `./`

| Commands          | Syntax                        |
| ----------------- | ----------------------------- |
| help              | `./help`                      |
| icon              | `./icon`                      |
| ping              | `./ping`                      |
| stat              | `./stat`                      |
| add_role_all_user | `./add_role_all_user ROLE_ID` |
| rm_role_all_user  | `./rm_role_all_user ROLE_ID`  |
| get_messages      | `./get_messages`              |
| update            | `./update`                    |

## 1.6. Latest major change
> Added bot update command

## 1.7. Latest minor change
> Updated and improved poll

## 1.8. To-do
- [x] Add no-mic roles to users when they join a vc to access hidden channel
- [ ] Implement blacklist words filter
- [ ] Add more information to stat command embed
- [x] Add logs for channel events
- [ ] Add logs for role events
- [x] Add logs for emoji events
- [ ] Add logs for moderation events
