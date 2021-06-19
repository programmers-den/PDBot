#pragma once

#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "bot_include.h"

void on_message_update(struct discord *client, const struct discord_user *bot, const struct discord_message *message);
