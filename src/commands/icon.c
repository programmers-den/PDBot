#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void icon(struct discord *client, const struct discord_message *msg) {
    if (msg->author->bot) return;

    char *author_avatar_url = malloc(AVATAR_URL_LEN);
    struct discord_embed embed;
    discord_embed_init(&embed);
    struct discord_create_message_params params = {.embed = &embed};

    embed.timestamp = cog_timestamp_ms();
    embed.color = COLOR_AERO;

    get_avatar_url(author_avatar_url, msg->author);
    discord_embed_set_author(&embed, msg->author->username, NULL, author_avatar_url, NULL);
    discord_embed_set_image(&embed, author_avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    
    discord_create_message(client, msg->channel_id, &params, NULL);

    free(author_avatar_url);
    discord_embed_cleanup(&embed);

    return;
}
