#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

void on_guild_member_remove(struct discord *client, const struct discord_user *bot, const u64_snowflake_t guild_id, const struct discord_user *user) {
    char *icon_url = malloc(AVATAR_URL_LEN), user_id_str[ID_STR_LEN], user_str[USER_MENTION_LEN], username_and_discriminator[USER_AND_DESCRIM_LEN], timestamp_str[TIMESTAMP_NORMAL_STR_LEN];
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    get_icon_url(icon_url, user);
    id_to_str(user_id_str, user->id);
    user_mention(user_str, user->id);
    username_and_discriminator_to_str(username_and_discriminator, user);
    cee_timestamp_str(timestamp_str, TIMESTAMP_NORMAL_STR_LEN);

    embed->color = COLOR_RED;
    embed->timestamp = cee_timestamp_ms();

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
    discord_embed_free(embed);

    return;
}
