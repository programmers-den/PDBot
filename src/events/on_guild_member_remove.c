#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void on_guild_member_remove(struct discord *client, const struct discord_guild_member_remove *event) {
    char user_id_str[ID_STR_LEN], user_str[USER_MENTION_LEN], username_and_discriminator[USER_AND_DESCRIM_LEN], timestamp_str[TIMESTAMP_NORMAL_STR_LEN], footer_text[ID_STR_LEN+4];
    char *avatar_url = malloc(AVATAR_URL_LEN);
    struct discord_embed embed;
    discord_embed_init(&embed);
    struct discord_create_message params = {
        .embeds = &(struct discord_embeds) { 
            .size = 1,
            .array = &embed
        }
    };

    get_avatar_url(avatar_url, event->user);
    id_to_str(user_id_str, event->user->id);
    user_mention(user_str, event->user->id);
    username_and_discriminator_to_str(username_and_discriminator, event->user);
    snprintf(timestamp_str, TIMESTAMP_NORMAL_STR_LEN, "<t:%lu>", cog_timestamp_ms()/1000);

    embed.color = COLOR_RED;
    embed.timestamp = cog_timestamp_ms();

    discord_embed_set_author(&embed, event->user->username, NULL, avatar_url, NULL);
    discord_embed_set_thumbnail(&embed, avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    snprintf(footer_text, sizeof(footer_text), "ID: %lu", event->user->id);
    discord_embed_set_footer(&embed, footer_text, avatar_url, NULL);
    discord_embed_set_title(&embed, "User left %s", username_and_discriminator);
    discord_embed_set_description(&embed, "");
    discord_embed_add_field(&embed, "User ID", user_id_str, true);
    discord_embed_add_field(&embed, "User", user_str, true);
    discord_embed_add_field(&embed, "Left at ", timestamp_str, true);

    discord_create_message(client, C_LOG, &params, NULL);

    free(avatar_url);
    discord_embed_cleanup(&embed);

    return;
}
