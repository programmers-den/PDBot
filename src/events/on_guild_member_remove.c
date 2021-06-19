#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"
#include "../libs/get_icon_url.h"
#include "../libs/format_message.h"

void on_guild_member_remove(struct discord *client, const struct discord_user *bot, const u64_snowflake_t guild_id, const struct discord_user *user) {
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};
    char *icon_url = get_icon_url(user);
    char *user_id_str = id_to_str(user->id);
    char *user_str = user_mention(user->id);
    char *username_and_discriminator = username_and_discriminator_to_str(user);

    embed->color = COLOR_RED;
    embed->timestamp = orka_timestamp_ms();

    char *timestamp_str = timestamp_to_str(embed->timestamp);

    discord_embed_set_author(embed, (char*)user->username, NULL, icon_url, NULL);
    discord_embed_set_thumbnail(embed, icon_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    snprintf(embed->footer->text, 2049, "ID: %lu", user->id);
    snprintf(embed->title, 257, "User left %s", username_and_discriminator);
    snprintf(embed->description, 2049, "");
    discord_embed_add_field(embed, "User ID", user_id_str, true);
    discord_embed_add_field(embed, "User", user_str, true);
    discord_embed_add_field(embed, "Left at ", timestamp_str, true);

    discord_create_message(client, C_LOG, &params, NULL);

    free(icon_url);
    free(user_id_str);
    free(user_str);
    free(username_and_discriminator);
    free(timestamp_str);
    discord_embed_free(embed);

    return;
}
