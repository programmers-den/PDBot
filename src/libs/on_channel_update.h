#pragma once

#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "bot_include.h"

void on_channel_update(struct discord *client, const struct discord_channel *channel);
