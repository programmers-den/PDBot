#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

void on_guild_member_remove(struct discord *client, const struct discord_user *bot, const u64_snowflake_t guild_id, const struct discord_user *user) {
    char user_id_str[ID_STR_LEN], user_str[USER_MENTION_LEN], username_and_discriminator[USER_AND_DESCRIM_LEN], timestamp_str[TIMESTAMP_NORMAL_STR_LEN];
    char *avatar_url = malloc(AVATAR_URL_LEN);
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    get_avatar_url(avatar_url, user);
    id_to_str(user_id_str, user->id);
    user_mention(user_str, user->id);
    username_and_discriminator_to_str(username_and_discriminator, user);
    cee_timestamp_str(timestamp_str, TIMESTAMP_NORMAL_STR_LEN);

    embed->color = COLOR_RED;
    embed->timestamp = cee_timestamp_ms();

    discord_embed_set_author(embed, (char*)user->username, NULL, avatar_url, NULL);
    discord_embed_set_thumbnail(embed, avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    snprintf(embed->footer->text, sizeof(embed->footer->text), "ID: %lu", user->id);
    snprintf(embed->title, sizeof(embed->title), "User left %s", username_and_discriminator);
    snprintf(embed->description, sizeof(embed->description), "");
    discord_embed_add_field(embed, "User ID", user_id_str, true);
    discord_embed_add_field(embed, "User", user_str, true);
    discord_embed_add_field(embed, "Left at ", timestamp_str, true);

    discord_create_message(client, C_LOG, &params, NULL);

    free(avatar_url);
    discord_embed_free(embed);

    return;
}
