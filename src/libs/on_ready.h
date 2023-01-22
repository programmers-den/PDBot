#pragma once

#include <concord/discord.h>
#include "bot_include.h"

void on_ready(struct discord *client, const struct discord_ready *event);
