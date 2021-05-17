#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <orca/discord.h>
#include <orca/log.h>
#include "config.h"
#include "get_icon_url.h"

void on_message_create(struct discord *client, const struct discord_user *bot, const struct discord_message *message);
