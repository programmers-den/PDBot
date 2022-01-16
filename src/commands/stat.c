#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void stat(struct discord *client, const struct discord_message *message) {
    if (message->author->bot) return;

    char user_id[ID_STR_LEN];
    char *author_avatar_url = malloc(AVATAR_URL_LEN);
    struct discord_embed embed;
    discord_embed_init(&embed);
    struct discord_create_message_params params = {.embed = &embed};

    get_avatar_url(author_avatar_url, message->author);
    snprintf(user_id, ID_STR_LEN, "%lu", message->author->id);

    embed.timestamp = cog_timestamp_ms();
    discord_embed_set_author(&embed, message->author->username, NULL, author_avatar_url, NULL);
    discord_embed_set_image(&embed, author_avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    discord_embed_add_field(&embed, "ID", user_id, false);
    discord_create_message(client, message->channel_id, &params, NULL);

    free(author_avatar_url);
    discord_embed_cleanup(&embed);

    return;
}
