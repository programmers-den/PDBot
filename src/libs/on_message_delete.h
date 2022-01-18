#pragma once

#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "bot_include.h"

void on_message_delete(struct discord *client, const u64_snowflake_t id, const u64_snowflake_t channel_id, const u64_snowflake_t guild_id);
