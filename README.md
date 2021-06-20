# 1. ~$ PDBot
![PD logo](https://cdn.discordapp.com/icons/668000598221651975/9c38858cf48263dbbc5ef4b3315bdc6d.webp?size=64)

PDBot is the official Discord bot for the [Programmer's Den](https://pden.net) server. It provides many utilities for the management, moderation, and members of this server

## 1.1. Dependencies and install instructions
> [orca](https://github.com/cee-studio/orca)
```bash
apt install -y git build-essential libcurl4-openssl-dev libssl-dev

git clone "https://github.com/json-c/json-c.git"

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
cmake ../json-c
make -j$(nproc) # or number of threads
make test
make install
```
> [sqlite3](https://sqlite.org/index.html)
```bash
apt install -y libsqlite3-dev
```

## 1.2. Bot config file
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

## 1.3. Commands
> The prefix is `./`

| Commands | Syntax   |
| -------- | -------- |
| help     | `./help` |
| icon     | `./icon` |
| ping     | `./ping` |
| stat     | `./stat` |

## 1.4. Latest major change
> Implemented no-mic channel roles when joining a vc

## 1.5. Latest minor change
> Fixed segfault when user join and left due to mistake in code

## 1.6. To-do
- [x] Add no-mic roles to users when they join a vc to access hidden channel
- [ ] Implement blacklist words filter
- [ ] Add more information to stat command embed
- [ ] Add logs for channel events
- [ ] Add logs for role events
- [ ] Add logs for emoji events
- [ ] Add logs for moderation events
