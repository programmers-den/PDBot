#pragma once

#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_message_reaction_add(struct discord *client, const u64snowflake user_id, const u64snowflake channel_id, const u64snowflake message_id, const u64snowflake guild_id, const struct discord_guild_member *member, const struct discord_emoji *emoji);
