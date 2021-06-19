#pragma once

#include <sqlite3.h>
#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "bot_include.h"

void check_for_db();
struct discord_message *fetch_message_db(struct discord *client, u64_snowflake_t guild_id, u64_snowflake_t message_id);
void add_message_db(const struct discord_message *message);
void remove_message_db(u64_snowflake_t message_id);
void update_message_db(const struct discord_message *message);
