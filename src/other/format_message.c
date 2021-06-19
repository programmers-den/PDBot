#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

void id_to_str(char buf[], u64_snowflake_t id) {
    // buf[ID_STR_LEN]
    snprintf(buf, ID_STR_LEN, "%lu", id);

    return;
}

void timestamp_to_str(char buf[], u64_unix_ms_t timestamp) {
    // buf[TIMESTAMP_STR_LEN]
    cee_timestamp_str(buf, TIMESTAMP_STR_LEN);

    return;
}

void message_mention(char buf[], char *mention_label, const struct discord_message *message) {
    // buf[MESSAGE_MENTION_LEN]
    char message_url[MESSAGE_URL_LEN];

    get_message_url(message_url, message);
    snprintf(buf, strlen(mention_label)+MESSAGE_URL_LEN+9, "**[%s](%s)**", mention_label, message_url);

    return;
}

void user_mention(char buf[], u64_snowflake_t user_id) {
    // buf[USER_MENTION_LEN]
    snprintf(buf, USER_MENTION_LEN, "<@%lu>", user_id);

    return;
}

void user_nick_mention(char buf[], u64_snowflake_t user_id) {
    // buf[USER_NICK_MENTION_LEN]
    snprintf(buf, USER_NICK_MENTION_LEN, "<@!%lu>", user_id);

    return;
}

void channel_mention(char buf[], u64_snowflake_t channel_id) {
    // buf[CHANNEL_MENTiON_LEN]
    snprintf(buf, CHANNEL_MENTiON_LEN, "<#%lu>", channel_id);

    return;
}

size_t emoji_mention_len(const struct discord_emoji *emoji) {
    if (emoji->animated) return strlen(emoji->name)+ID_LEN+7+1;
    else return strlen(emoji->name)+ID_LEN+6+1;
}

void emoji_mention(char buf[], const struct discord_emoji *emoji) {
    // buf[emoji_mention_len(emoji)]
    size_t len = emoji_mention_len(emoji);
    if (emoji->animated) snprintf(buf, len, "<a:%s:%lu>", emoji->name, emoji->id);
    else snprintf(buf, len, "<:%s:%lu>", emoji->name, emoji->id);

    return;
}

void username_and_discriminator_to_str(char buf[], const struct discord_user *user) {
    // buf[USER_AND_DESCRIM_LEN]
    snprintf(buf, USER_AND_DESCRIM_LEN, "%s#%s", user->username, user->discriminator);

    return;
}
