#pragma once

#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "config.h"
#include "get_icon_url.h"

void on_guild_member_add(struct discord *client, const struct discord_user *bot, const u64_snowflake_t guild_id, const struct discord_guild_member *member);
