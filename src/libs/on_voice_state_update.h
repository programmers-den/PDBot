#pragma once

#include <orca/discord.h>

void on_voice_state_update(struct discord *client, const struct discord_user *bot, const struct discord_voice_state *vs);
