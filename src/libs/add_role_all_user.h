#pragma once

#include <orca/discord.h>
#include "../libs/bot_include.h"

void add_role_all_user(struct discord *client, const struct discord_user *bot, const struct discord_message *msg);
