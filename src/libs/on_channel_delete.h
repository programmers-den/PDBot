#pragma once

#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "bot_include.h"

void on_channel_delete(struct discord *client, const struct discord_user *bot, const struct discord_channel *channel);
