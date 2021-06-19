#include <string.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"
#include "../libs/get_message_url.h"

void id_to_str(char buf[], u64_snowflake_t id) {
    // buf[ID_STR_LEN]
    snprintf(buf, ID_STR_LEN, "%lu", id);

    return;
}

void timestamp_to_str(char buf[], u64_unix_ms_t timestamp) {
    // buf[TIMESTAMP_STR_LEN]
    orka_timestamp_str(buf, TIMESTAMP_STR_LEN);

    return;
}

void message_mention(char buf[], char *mention_label, const struct discord_message *message) {
    // buf[MESSAGE_URL_LEN]
    char message_url[MESSAGE_URL_LEN];

    get_message_url(message_url, message);
    snprintf(buf, strlen(mention_label)+MESSAGE_URL_LEN+9, "**[%s](%s)**", mention_label, message_url);

    return;
}

void channel_mention(char buf[], u64_snowflake_t channel_id) {
    // buf[CHANNEL_MENTiON_LEN]
    snprintf(buf, CHANNEL_MENTiON_LEN, "<#%lu>", channel_id);

    return;
}

void user_mention(char buf[], u64_snowflake_t user_id) {
    // buf[USER_MENTION_LEN]
    snprintf(buf, USER_MENTION_LEN, "<@%lu>", user_id);

    return;
}

void username_and_discriminator_to_str(char buf[], const struct discord_user *user) {
    // buf[USER_AND_DESCRIM_LEN]
    snprintf(buf, USER_AND_DESCRIM_LEN, "%s#%s", user->username, user->discriminator);

    return;
}
