#include <string.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"
#include "../libs/get_message_url.h"

char *id_to_str(u64_snowflake_t id) {
    char *id_str = malloc(ID_LENGTH+1);
    snprintf(id_str, ID_LENGTH+1, "%lu", id);

    return id_str;
}

char *timestamp_to_str(u64_unix_ms_t timestamp) {
    char *formatted_timestamp = malloc(TIMESTAMP_LENGTH+1);
    orka_timestamp_str(formatted_timestamp, TIMESTAMP_LENGTH);

    return formatted_timestamp;
}

char *message_mention(char *mention_label, const struct discord_message *message) {
    char *message_url = get_message_url(message);
    size_t mention_label_len = strlen(mention_label)+strlen(message_url)+9;

    char *formatted_message = malloc(mention_label_len);
    snprintf(formatted_message, mention_label_len, "**[%s](%s)**", mention_label, message_url);

    free(message_url);
    return formatted_message;
}

char *channel_mention(u64_snowflake_t channel_id) {
    char *formatted_channel = malloc(ID_LENGTH+4);
    snprintf(formatted_channel, CHANNEL_MENTiON_LENGTH+4, "<#%lu>", channel_id);

    return formatted_channel;
}

char *user_mention(u64_snowflake_t user_id) {
    char *formatted_user = malloc(ID_LENGTH+4);
    snprintf(formatted_user, ID_LENGTH+4, "<@%lu>", user_id);

    return formatted_user;
}

char *username_and_discriminator_to_str(const struct discord_user *user) {
    size_t username_and_discriminator_len = strlen(user->username+6);
    char *formatted_user = malloc(username_and_discriminator_len);
    snprintf(formatted_user, username_and_discriminator_len, "%s#%s", user->username, user->discriminator);

    return formatted_user;
}
