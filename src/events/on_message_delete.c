#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"
#include "../libs/get_icon_url.h"
#include "../libs/fetch_message_db.h"
#include "../libs/remove_message_db.h"
#include "../libs/format_message.h"

void on_message_delete(struct discord *client, const struct discord_user *bot, const u64_snowflake_t message_id, const u64_snowflake_t channel_id, const u64_snowflake_t guild_id) {
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};
    struct discord_message *message = fetch_message_db(client, guild_id, message_id);

    embed->color = COLOR_RED;
    embed->timestamp = orka_timestamp_ms();

    if (message->content[0]) {
        char *icon_url = get_icon_url(message->author);
        char *message_id_str = id_to_str(message_id);
        char *channel_id_str = id_to_str(message->id);
        char *channel_str = channel_mention(channel_id);
        char *author_id_str = id_to_str(message->author->id);
        char *author_str = user_mention(message->author->id);

        discord_embed_set_author(embed, message->author->username, NULL, icon_url, NULL);
        discord_embed_set_thumbnail(embed, icon_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);

        snprintf(embed->title, 257, "Deleted message by %s#%s", message->author->username, message->author->discriminator);
        discord_embed_add_field(embed, "Message ID", message_id_str, true);
        discord_embed_add_field(embed, "Channel ID", channel_id_str, true);
        discord_embed_add_field(embed, "Author ID", author_id_str, true);
        discord_embed_add_field(embed, "Channel", channel_str, false);
        discord_embed_add_field(embed, "Author", author_str, true);
        discord_embed_add_field(embed, "Content", message->content, false);
        snprintf(embed->footer->text, 2049, "Author ID: %lu", message->author->id);

        discord_create_message(client, C_LOG, &params, NULL);

        free(icon_url);
        free(message_id_str);
        free(channel_id_str);
        free(channel_str);
        free(author_id_str);
        free(author_str);
        discord_embed_free(embed);

        remove_message_db(message_id);
    }

    discord_message_free(message);

    return;
}
