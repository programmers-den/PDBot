#pragma once

#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "config.h"

void get_message_url(char buf[], const struct discord_message *message);
