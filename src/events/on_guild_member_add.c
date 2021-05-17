#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"
#include "../libs/get_icon_url.h"

void on_guild_member_add(struct discord *client, const struct discord_user *bot, const u64_snowflake_t guild_id, const struct discord_guild_member *member) {
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    embed->color = 10026904;
    embed->timestamp = member->joined_at;
    char *icon_url = get_icon_url(member->user);
    discord_embed_set_author(embed, member->user->username, NULL, icon_url, NULL);
    discord_embed_set_thumbnail(embed, icon_url, NULL, 64, 64);
    snprintf(embed->title, 257, "Welcome %s#%s!", member->user->username, member->user->discriminator);
    snprintf(embed->description, 2049, "Welcome **%s#%s** to PD! Please checkout <#%lu> and <#%lu> to get started!", member->user->username, member->user->discriminator, C_SERVER_INFO, C_ROLES);
    snprintf(embed->footer->text, 2049, "ID: %lu", member->user->id);

    discord_create_message(client, C_WELCOME, &params, NULL);

    snprintf(embed->title, 257, "New user %s", member->user->username);
    snprintf(embed->description, 2049, "User: %s#%s\nID: %lu\nJoined at: %lu", member->user->username, member->user->discriminator, member->user->id, member->joined_at);

    discord_create_message(client, C_LOG, &params, NULL);
    free(icon_url);

    return;
}
