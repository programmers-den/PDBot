#pragma once

#include <regex.h>
#include <concord/discord.h>
#include "bot_include.h"

void get_messages(struct discord *client, const struct discord_interaction *interaction);
