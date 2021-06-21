#pragma once

#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "bot_include.h"

void id_to_str(char buf[], u64_snowflake_t id);
void timestamp_to_str(char buf[], u64_unix_ms_t timestamp);
void message_mention(char buf[], char *mention_label, const struct discord_message *message);
void user_mention(char buf[], u64_snowflake_t user_id);
void user_nick_mention(char buf[], u64_snowflake_t user_id);
void channel_mention(char buf[], u64_snowflake_t channel_id);
void role_mention(char buf[], u64_snowflake_t role_id);
size_t emoji_mention_len(const struct discord_emoji *emoji);
void emoji_mention(char buf[], const struct discord_emoji *emoji);
void username_and_discriminator_to_str(char buf[], const struct discord_user *user);
