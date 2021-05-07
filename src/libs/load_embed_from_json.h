#pragma once

#include <stddef.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>

struct discord_embed* load_embed_from_json(char *filename);
