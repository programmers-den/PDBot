#pragma once

#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "bot_include.h"

void on_guild_member_remove(struct discord *client, const struct discord_user *bot, const u64_snowflake_t guild_id, const struct discord_user *user);
