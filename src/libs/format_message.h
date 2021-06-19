#include <string.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"
#include "../libs/get_message_url.h"

void id_to_str(char buf[], u64_snowflake_t id);
void timestamp_to_str(char buf[], u64_unix_ms_t timestamp);
void message_mention(char buf[], char *mention_label, const struct discord_message *message);
void channel_mention(char buf[], u64_snowflake_t channel_id);
void user_mention(char buf[], u64_snowflake_t user_id);
void username_and_discriminator_to_str(char buf[], const struct discord_user *user);
