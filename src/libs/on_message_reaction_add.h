#pragma once

#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_message_reaction_add(struct discord *client, const struct discord_message_reaction_add *event);
