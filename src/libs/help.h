#pragma once

#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "load_embed_from_json.h"

void help(struct discord *client, const struct discord_user *user, const struct discord_message *msg);
