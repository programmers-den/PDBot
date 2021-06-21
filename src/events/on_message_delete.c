#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

void on_message_delete(struct discord *client, const struct discord_user *bot, const u64_snowflake_t message_id, const u64_snowflake_t channel_id, const u64_snowflake_t guild_id) {
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};
    struct discord_message *message = fetch_message_db(client, guild_id, message_id);

    embed->color = COLOR_RED;
    embed->timestamp = cee_timestamp_ms();

    if (message->content[0]) {
        char *author_avatar_url = malloc(AVATAR_URL_LEN), message_id_str[ID_STR_LEN], channel_id_str[ID_STR_LEN], channel_str[CHANNEL_MENTiON_LEN], author_id_str[ID_STR_LEN], author_str[USER_MENTION_LEN], username_and_discriminator[USER_AND_DESCRIM_LEN];

        get_avatar_url(author_avatar_url, message->author);
        id_to_str(message_id_str, message_id);
        id_to_str(channel_id_str, message->id);
        channel_mention(channel_str, channel_id);
        id_to_str(author_id_str, message->author->id);
        user_mention(author_str, message->author->id);
        username_and_discriminator_to_str(username_and_discriminator, message->author);

        snprintf(embed->title, 257, "Deleted message by %s", username_and_discriminator);

        discord_embed_set_author(embed, message->author->username, NULL, author_avatar_url, NULL);
        discord_embed_set_thumbnail(embed, author_avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
        discord_embed_add_field(embed, "Message ID", message_id_str, true);
        discord_embed_add_field(embed, "Channel ID", channel_id_str, true);
        discord_embed_add_field(embed, "Author ID", author_id_str, true);
        discord_embed_add_field(embed, "Channel", channel_str, false);
        discord_embed_add_field(embed, "Author", author_str, true);
        discord_embed_add_field(embed, "Content", message->content, false);
        snprintf(embed->footer->text, 2049, "Author ID: %lu", message->author->id);

        discord_create_message(client, C_LOG, &params, NULL);

        free(author_avatar_url);
        discord_embed_free(embed);

        remove_message_db(message_id);
    }

    discord_message_free(message);

    return;
}
