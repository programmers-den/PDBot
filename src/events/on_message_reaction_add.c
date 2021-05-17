#include <orca/discord.h>
#include "../libs/config.h"
#include "../libs/get_icon_url.h"
#include "../libs/get_message_url.h"

void on_message_reaction_add(struct discord *client, const struct discord_user *bot, const u64_snowflake_t user_id, const u64_snowflake_t channel_id, const u64_snowflake_t message_id, const u64_snowflake_t guild_id, const struct discord_guild_member *member, const struct discord_emoji *emoji) {
    struct discord_message *message = discord_message_alloc();
    discord_get_channel_message(client, channel_id, message_id, message);
    size_t count;
    for (size_t i=0; message->reactions[i]; i++) if (!strcmp(message->reactions[i]->emoji->name, E_STAR)) count = message->reactions[i]->count;
    if (count == STARBOARD_REACTION_COUNT && !strcmp(emoji->name, E_STAR)) {
        struct discord_embed *embed = discord_embed_alloc();
        struct discord_create_message_params params = {.embed = embed};

        embed->color = COLOR_YELLOW;
        embed->timestamp = orka_timestamp_ms();
        char *icon_url = get_icon_url(member->user);
        char *url = get_message_url(guild_id, message);
        discord_embed_set_author(embed, (char*)member->user->username, NULL, icon_url, NULL);
        discord_embed_set_thumbnail(embed, icon_url, NULL, 64, 64);
        snprintf(embed->description, 2049, "%s\n\n**[Jump to message](%s)**", message->content, url);

        discord_create_message(client, C_STARBOARD, &params, NULL);
        free(icon_url);
        free(url);
        discord_embed_free(embed);
    }

    discord_message_free(message);

    return;
}
