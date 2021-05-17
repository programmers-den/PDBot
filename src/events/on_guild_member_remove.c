#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

void on_guild_member_remove(struct discord *client, const struct discord_user *bot, const u64_snowflake_t guild_id, const struct discord_user *user) {
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    embed->color = 16711680;
    embed->timestamp = orka_timestamp_ms();
    // discord_embed_set_author(embed, user->username, NULL, user->avatar, NULL);
    // discord_embed_set_image(embed, user->avatar, NULL, 64, 64);
    snprintf(embed->title, 257, "User left **%s#%s**", user->username, user->discriminator);
    snprintf(embed->description, 2049, "User: %s#%s\nID: %d\nLeft at: %d", user->username, user->discriminator, user->id, orka_timestamp_ms());

    discord_create_message(client, C_LOG, &params, NULL);

    return;
}
