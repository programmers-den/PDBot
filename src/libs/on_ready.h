#pragma once

#include <stdio.h>
#include <orca/discord.h>
#include <json-c/json.h>
#include "../libs/get_commands.h"

void on_ready(struct discord *client, const struct discord_user *user);
