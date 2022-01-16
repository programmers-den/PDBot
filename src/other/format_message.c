#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void id_to_str(char *buf, u64_snowflake_t id) {
    // buf[ID_STR_LEN]
    memset(buf, 0, ID_STR_LEN);
    snprintf(buf, ID_STR_LEN, "%lu", id);

    return;
}

void timestamp_to_str(char *buf, u64_unix_ms_t timestamp) {
    // buf[TIMESTAMP_STR_LEN]
    memset(buf, 0, TIMESTAMP_STR_LEN);
    snprintf(buf, TIMESTAMP_STR_LEN, "%lu", timestamp);

    return;
}

void message_mention(char *buf, char *mention_label, const struct discord_message *message) {
    // buf[MESSAGE_MENTION_LEN]
    memset(buf, 0, MESSAGE_MENTION_LEN);
    char message_url[MESSAGE_URL_LEN];

    get_message_url(message_url, message);
    snprintf(buf, strlen(mention_label)+MESSAGE_URL_LEN+9, "**[%s](%s)**", mention_label, message_url);

    return;
}

void user_mention(char *buf, u64_snowflake_t user_id) {
    // buf[USER_MENTION_LEN]
    memset(buf, 0, USER_MENTION_LEN);
    snprintf(buf, USER_MENTION_LEN, "<@%lu>", user_id);

    return;
}

void user_nick_mention(char *buf, u64_snowflake_t user_id) {
    // buf[USER_NICK_MENTION_LEN]
    memset(buf, 0, USER_NICK_MENTION_LEN);
    snprintf(buf, USER_NICK_MENTION_LEN, "<@!%lu>", user_id);

    return;
}

void channel_mention(char *buf, u64_snowflake_t channel_id) {
    // buf[CHANNEL_MENTiON_LEN]
    memset(buf, 0, CHANNEL_MENTiON_LEN);
    snprintf(buf, CHANNEL_MENTiON_LEN, "<#%lu>", channel_id);

    return;
}

void role_mention(char *buf, u64_snowflake_t role_id) {
    // buf[ROLE_MENTION_LEN]
    memset(buf, 0, ROLE_MENTION_LEN);
    snprintf(buf, ROLE_MENTION_LEN, "<@&%lu>", role_id);

    return;
}

void role_list_mention(char **buf, u64_snowflake_t *role_ids) {
    // *buf[ROLE_MENTION_LEN]
    for (size_t i=0; role_ids[i]; i++) {
        memset(buf[i], 0, ROLE_MENTION_LEN);
        snprintf(buf[i], ROLE_MENTION_LEN, "<@%lu>", role_ids[i]);
    }

    return;
}

size_t emoji_mention_len(const struct discord_emoji *emoji) {
    if (emoji->animated) return strlen(emoji->name)+ID_LEN+7+1;
    else return strlen(emoji->name)+ID_LEN+6+1;
}

void emoji_mention(char *buf, const struct discord_emoji *emoji) {
    // buf[emoji_mention_len(emoji)]
    size_t len = emoji_mention_len(emoji);
    memset(buf, 0, len);
    if (emoji->animated) snprintf(buf, len, "<a:%s:%lu>", emoji->name, emoji->id);
    else snprintf(buf, len, "<:%s:%lu>", emoji->name, emoji->id);

    return;
}

void username_and_discriminator_to_str(char *buf, const struct discord_user *user) {
    // buf[USER_AND_DESCRIM_LEN]
    memset(buf, 0, USER_AND_DESCRIM_LEN);
    snprintf(buf, USER_AND_DESCRIM_LEN, "%s#%s", user->username, user->discriminator);

    return;
}

void failed_message(struct discord *client, struct discord_embed *embed, struct discord_create_message_params *params, char *reason, u64_snowflake_t channel_id) {
    embed->color = COLOR_RED;
    snprintf(embed->title, sizeof(embed->title), "Failed!");
    snprintf(embed->description, sizeof(embed->description), "Reason: %s", reason);

    discord_create_message(client, channel_id, params, NULL);
}
