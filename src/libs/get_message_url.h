#pragma once

#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

char *get_message_url(const u64_snowflake_t guild_id, struct discord_message *message);
