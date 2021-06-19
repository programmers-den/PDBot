#include <string.h>
#include <orca/discord.h>
#include "config.h"
#include "get_message_url.h"

char *id_to_str(u64_snowflake_t id);
char *timestamp_to_str(u64_unix_ms_t timestamp);
char *message_mention(char *mention_label, const struct discord_message *message);
char *channel_mention(u64_snowflake_t channel_id);
char *user_mention(u64_snowflake_t user_id);
char *username_and_discriminator_to_str(const struct discord_user *user);
