#include <regex.h>
#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "bot_include.h"

size_t get_args_len(const struct discord_message *message, char *split);
uint8_t get_arg_at(char *buf, const struct discord_message *message, size_t argc, char *split);
size_t check_if_args_ids(const struct discord_message *message, char *split);
u64snowflake get_args_ids_as_int_at(const struct discord_message *message, size_t argc, char *split);
uint8_t get_args_ids_as_str_at(char *buf, const struct discord_message *message, size_t argc, char *split);
