#pragma once

#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "bot_include.h"

void on_help(struct discord *client, const struct discord_user *user, const struct discord_message *msg);
