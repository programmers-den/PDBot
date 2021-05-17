#pragma once

#include <orca/discord.h>
#include "config.h"

void on_message_reaction_remove(struct discord *client, const struct discord_user *bot, const u64_snowflake_t user_id, const u64_snowflake_t channel_id, const u64_snowflake_t message_id, const u64_snowflake_t guild_id, const struct discord_emoji *emoji);
