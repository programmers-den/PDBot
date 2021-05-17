#include <orca/discord.h>
#include "../libs/config.h"

void on_message_reaction_remove(struct discord *client, const struct discord_user *bot, const u64_snowflake_t user_id, const u64_snowflake_t channel_id, const u64_snowflake_t message_id, const u64_snowflake_t guild_id, const struct discord_emoji *emoji) {
    // struct discord_message *message = discord_message_alloc();
    // discord_get_channel_message(client, channel_id, message_id, message);
    // size_t count;
    // for (size_t i=0; message->reactions[i]; i++) if (!strcmp(message->reactions[i]->emoji->name, E_STAR)) count = message->reactions[i]->count;
    // if (count == STARBOARD_REACTION_COUNT && !strcmp(emoji->name, E_STAR)) discord_delete_message(client, channel_id, message_id);

    // discord_message_free(message);

    return;
}
