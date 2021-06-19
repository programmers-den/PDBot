#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"
#include "../libs/get_icon_url.h"
#include "../libs/format_message.h"

void on_guild_member_add(struct discord *client, const struct discord_user *bot, const u64_snowflake_t guild_id, const struct discord_guild_member *member) {
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};
    char *icon_url = get_icon_url(member->user);
    char *username_and_discriminator = username_and_discriminator_to_str(member->user);
    char *user_id_str = id_to_str(member->user->id);
    char *user_str = user_mention(member->user->id);
    char *timestamp_str = timestamp_to_str(member->joined_at);

    embed->color = COLOR_MINT;
    embed->timestamp = member->joined_at;

    discord_embed_set_author(embed, member->user->username, NULL, icon_url, NULL);
    discord_embed_set_thumbnail(embed, icon_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    snprintf(embed->title, 257, "Welcome %s!", username_and_discriminator);
    snprintf(embed->description, 2049, "Welcome **%s** to PD! Please checkout <#%lu> and <#%lu> to get started!", username_and_discriminator, C_SERVER_INFO, C_ROLES);
    snprintf(embed->footer->text, 2049, "ID: %lu", member->user->id);

    discord_create_message(client, C_WELCOME, &params, NULL);

    snprintf(embed->title, 257, "New user %s", username_and_discriminator);
    snprintf(embed->description, 2049, "");
    discord_embed_add_field(embed, "User ID", user_id_str, true);
    discord_embed_add_field(embed, "User", user_str, true);
    discord_embed_add_field(embed, "Joined at ", timestamp_str, true);

    discord_create_message(client, C_LOG, &params, NULL);

    free(icon_url);
    free(username_and_discriminator);
    free(user_id_str);
    free(user_str);
    free(timestamp_str);
    discord_embed_free(embed);

    return;
}
