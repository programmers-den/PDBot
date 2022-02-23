#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void on_channel_delete(struct discord *client, const struct discord_channel *channel) {
    char channel_id_str[ID_STR_LEN], channel_str[CHANNEL_MENTION_LEN];
    char *avatar_url = malloc(AVATAR_URL_LEN);
    struct discord_embed embed;
    discord_embed_init(&embed);
    struct discord_create_message params = {
        .embeds = &(struct discord_embeds) {
            .size = 1,
            .array = &embed
        }
    };

    embed.timestamp = cog_timestamp_ms();
    embed.color = COLOR_PINK_PANTONE;

    id_to_str(channel_id_str, channel->id);
    channel_mention(channel_str, channel->id);

    discord_embed_set_title(&embed, "Channel deleted");
    discord_embed_add_field(&embed, "Channel name", (char*)channel->name, true);
    discord_embed_add_field(&embed, "Channel ID", channel_id_str, true);
    discord_embed_add_field(&embed, "Channel", channel_str, true);

    discord_create_message(client, C_LOG, &params, NULL);

    free(avatar_url);
    discord_embed_cleanup(&embed);

    return;
}
