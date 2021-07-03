#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

void on_message_update(struct discord *client, const struct discord_user *bot, const struct discord_message *message) {
    if (message->author->bot) return;

    char *author_avatar_url = malloc(AVATAR_URL_LEN), message_id_str[ID_STR_LEN], message_str[MESSAGE_URL_LEN], channel_id_str[ID_STR_LEN], channel_str[CHANNEL_MENTiON_LEN], author_id_str[ID_STR_LEN], author_str[USER_MENTION_LEN], username_and_discriminator[USER_AND_DESCRIM_LEN];
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};
    struct discord_message *db_message = fetch_message_db(client, message->guild_id, message->id);

    get_avatar_url(author_avatar_url, message->author);
    id_to_str(message_id_str, message->id);
    message_mention(message_str, "Jump to message", message);
    id_to_str(channel_id_str, message->channel_id);
    channel_mention(channel_str, message->channel_id);
    id_to_str(author_id_str, message->author->id);
    user_mention(author_str, message->author->id);
    username_and_discriminator_to_str(username_and_discriminator, message->author);

    if (db_message->content[0]) update_message_db(message);
    else add_message_db(message);

    embed->color = COLOR_YELLOW;
    embed->timestamp = cee_timestamp_ms();
    discord_embed_set_author(embed, message->author->username, NULL, author_avatar_url, NULL);
    discord_embed_set_thumbnail(embed, author_avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    snprintf(embed->footer->text, 2049, "Author ID: %lu", message->author->id);
    snprintf(embed->title, 257, "Edit message by %s", username_and_discriminator);
    discord_embed_add_field(embed, "Message ID", message_id_str, true);
    discord_embed_add_field(embed, "Channel ID", channel_id_str, true);
    discord_embed_add_field(embed, "Author ID", author_id_str, true);
    discord_embed_add_field(embed, "Message", message_str, false);
    discord_embed_add_field(embed, "Channel", channel_str, true);
    discord_embed_add_field(embed, "Author", author_str, true);
    if (db_message->content[0]) discord_embed_add_field(embed, "Previous content", db_message->content, false);
    discord_embed_add_field(embed, "New content", message->content, false);

    discord_create_message(client, C_LOG, &params, NULL);

    free(author_avatar_url);
    discord_embed_free(embed);
    discord_message_free(db_message);

    return;
}
