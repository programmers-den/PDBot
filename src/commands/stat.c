#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"
#include "../libs/get_icon_url.h"

void stat(struct discord *client, const struct discord_user *bot, const struct discord_message *message) {
    if (message->author->bot) return;

    char *icon_url = malloc(AVATAR_URL_LEN), user_id[ID_STR_LEN];
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    get_icon_url(icon_url, message->author);
    snprintf(user_id, ID_STR_LEN, "%lu", message->author->id);

    embed->timestamp = orka_timestamp_ms();
    discord_embed_set_author(embed, message->author->username, NULL, icon_url, NULL);
    discord_embed_set_image(embed, icon_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    discord_embed_add_field(embed, "ID", user_id, false);
    discord_create_message(client, message->channel_id, &params, NULL);

    free(icon_url);
    discord_embed_free(embed);

    return;
}
