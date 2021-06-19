#pragma once

#include <orca/discord.h>

void on_ping(struct discord *client, const struct discord_user *user, const struct discord_message *msg);
