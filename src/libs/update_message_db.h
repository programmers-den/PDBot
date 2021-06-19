#pragma once

#include <orca/discord.h>
#include <sqlite3.h>
#include "../libs/config.h"

void update_message_db(const struct discord_message *message);
