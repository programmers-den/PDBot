#pragma once

#include <orca/discord.h>
#include "config.h"
#include "get_icon_url.h"
#include "get_message_url.h"
#include "format_message.h"

void on_message_reaction_add(struct discord *client, const struct discord_user *bot, const u64_snowflake_t user_id, const u64_snowflake_t channel_id, const u64_snowflake_t message_id, const u64_snowflake_t guild_id, const struct discord_guild_member *member, const struct discord_emoji *emoji);
