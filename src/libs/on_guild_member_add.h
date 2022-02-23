#pragma once

#include <concord/discord.h>
#include "bot_include.h"

void on_guild_member_add(struct discord *client, const u64snowflake guild_id, const struct discord_guild_member *member);
