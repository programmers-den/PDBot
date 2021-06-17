#include <sqlite3.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "config.h"

struct discord_message *fetch_message_db(struct discord *client, u64_snowflake_t guild_id, u64_snowflake_t message_id);
