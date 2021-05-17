#include <orca/discord.h>
#include "../libs/config.h"
#include "../libs/get_icon_url.h"

void icon(struct discord *client, const struct discord_user *user, const struct discord_message *msg) {
    if (msg->author->bot) return;

    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    embed->timestamp = orka_timestamp_ms();
    embed->color = COLOR_BLUE;
    char *icon_url = get_icon_url(msg->author);
    discord_embed_set_author(embed, msg->author->username, NULL, icon_url, NULL);
    discord_embed_set_image(embed, icon_url, NULL, 64, 64);
    
    discord_create_message(client, msg->channel_id, &params, NULL);
    free(icon_url);

    return;
}
