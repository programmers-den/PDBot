#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

void on_channel_create(struct discord *client, const struct discord_user *bot, const struct discord_channel *channel) {
    char channel_id_str[ID_STR_LEN], channel_str[CHANNEL_MENTiON_LEN];
    char *avatar_url = malloc(AVATAR_URL_LEN)
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    embed->timestamp = cee_timestamp_ms();
    embed->color = COLOR_LIGHT_GREEN;

    id_to_str(channel_id_str, channel->id);
    channel_mention(channel_str, channel->id);

    snprintf(embed->title, 257, "Channel created");
    discord_embed_add_field(embed, "Channel name", (char*)channel->name, true);
    discord_embed_add_field(embed, "Channel ID", channel_id_str, true);
    discord_embed_add_field(embed, "Channel", channel_str, true);

    discord_create_message(client, C_LOG, &params, NULL);

    free(avatar_url);
    discord_embed_free(embed);

    return;
}
