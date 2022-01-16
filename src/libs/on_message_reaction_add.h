#pragma once

#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_message_reaction_add(struct discord *client, const u64_snowflake_t user_id, const u64_snowflake_t channel_id, const u64_snowflake_t message_id, const u64_snowflake_t guild_id, const struct discord_guild_member *member, const struct discord_emoji *emoji);
