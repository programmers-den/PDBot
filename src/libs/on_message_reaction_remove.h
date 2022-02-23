#pragma once

#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "bot_include.h"

void on_message_reaction_remove(struct discord *client, const u64snowflake user_id, const u64snowflake channel_id, const u64snowflake message_id, const u64snowflake guild_id, const struct discord_emoji *emoji);
