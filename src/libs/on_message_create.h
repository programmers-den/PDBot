#pragma once

#include <concord/discord.h>
#include "bot_include.h"

void on_message_create(struct discord *client, const struct discord_message *message);
