#pragma once

#include <sqlite3.h>
#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "bot_include.h"

extern sqlite3 *db;
extern FILE *dbFile;
extern int dbIsOpen;

void ready_db();
void close_db();
struct discord_message fetch_message_db(struct discord *client, u64snowflake guild_id, u64snowflake message_id);
void add_message_db(const struct discord_message *message);
void remove_message_db(u64snowflake message_id);
void update_message_db(const struct discord_message *message);
