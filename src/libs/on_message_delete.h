#pragma once

#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

void on_message_delete(struct discord *client, const struct discord_user *bot, const u64_snowflake_t id, const u64_snowflake_t channel_id, const u64_snowflake_t guild_id);
