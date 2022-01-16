#pragma once

#include <concord/discord.h>

uint8_t guild_member_has_role(struct discord_guild_member *guild_member, u64_snowflake_t role_id);
