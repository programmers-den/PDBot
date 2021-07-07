#pragma once

#include <orca/discord.h>
#include "bot_include.h"

void get_messages(struct discord *client, const struct discord_user *bot, const struct discord_message *msg);
