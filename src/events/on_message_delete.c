#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void on_message_delete(struct discord *client, const u64_snowflake_t message_id, const u64_snowflake_t channel_id, const u64_snowflake_t guild_id) {
    struct discord_embed embed;
    discord_embed_init(&embed);
    struct discord_create_message_params params = {.embed = &embed};
    struct discord_message message = fetch_message_db(client, guild_id, message_id);

    embed.color = COLOR_RED;
    embed.timestamp = cog_timestamp_ms();

    if (!message.content[0]) {
        discord_embed_cleanup(&embed);
        discord_message_cleanup(&message);
        return;
    }
    else if (message.content[0]) {
        char message_id_str[ID_STR_LEN], channel_id_str[ID_STR_LEN], channel_str[CHANNEL_MENTiON_LEN], author_id_str[ID_STR_LEN], author_str[USER_MENTION_LEN], username_and_discriminator[USER_AND_DESCRIM_LEN];
        char *author_avatar_url = malloc(AVATAR_URL_LEN);

        get_avatar_url(author_avatar_url, message.author);
        id_to_str(message_id_str, message_id);
        id_to_str(channel_id_str, message.id);
        channel_mention(channel_str, channel_id);
        id_to_str(author_id_str, message.author->id);
        user_mention(author_str, message.author->id);
        username_and_discriminator_to_str(username_and_discriminator, message.author);

        snprintf(embed.title, sizeof(embed.title), "Deleted message by %s", username_and_discriminator);

        discord_embed_set_author(&embed, message.author->username, NULL, author_avatar_url, NULL);
        discord_embed_set_thumbnail(&embed, author_avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
        discord_embed_add_field(&embed, "Message ID", message_id_str, true);
        discord_embed_add_field(&embed, "Channel ID", channel_id_str, true);
        discord_embed_add_field(&embed, "Author ID", author_id_str, true);
        discord_embed_add_field(&embed, "Channel", channel_str, false);
        discord_embed_add_field(&embed, "Author", author_str, true);
        discord_embed_add_field(&embed, "Content", message.content, false);
        snprintf(embed.footer->text, sizeof(embed.footer->text), "Author ID: %lu", message.author->id);

        discord_create_message(client, C_LOG, &params, NULL);

        free(author_avatar_url);
        discord_embed_cleanup(&embed);
        discord_message_cleanup(&message);

        remove_message_db(message_id);
    }


    return;
}
