#pragma once

#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "bot_include.h"

void on_guild_member_remove(struct discord *client, const u64snowflake guild_id, const struct discord_user *user);
