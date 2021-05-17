#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/get_icon_url.h"

void stat(struct discord *client, const struct discord_user *bot, const struct discord_message *message) {
    if (message->author->bot) return;

    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};
    char *icon_url = get_icon_url(message->author);
    char *user_id = malloc(19);
    snprintf(user_id, 19, "%lu", message->author->id);

    embed->timestamp = orka_timestamp_ms();
    discord_embed_set_author(embed, message->author->username, NULL, icon_url, NULL);
    discord_embed_set_image(embed, icon_url, NULL, 64, 64);
    discord_embed_add_field(embed, "ID", user_id, false);
    discord_create_message(client, message->channel_id, &params, NULL);

    free(icon_url);
    discord_embed_free(embed);

    return;
}
