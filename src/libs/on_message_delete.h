#pragma once

#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "bot_include.h"

void on_message_delete(struct discord *client, const u64snowflake id, const u64snowflake channel_id, const u64snowflake guild_id);
