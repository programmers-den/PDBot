#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_guild_member_add(struct discord *client, const struct discord_guild_member *event) {
    char username_and_discriminator[DISCORD_MAX_USERNAME_LEN], user_id_str[ID_STR_LEN], timestamp_str[TIMESTAMP_NORMAL_STR_LEN], footer_text[ID_STR_LEN+4];
    char *avatar_url = malloc(AVATAR_URL_LEN), *user_mention_str = malloc(USER_MENTION_LEN);
    struct discord_embed embed;
    discord_embed_init(&embed);
    struct discord_create_message message_params = {
        .embeds = &(struct discord_embeds) {
            .size = 1,
            .array = &embed
        }    
    };
    struct discord_add_guild_member_role role_params = {
        .reason = "Server member default roles"
    };

    get_avatar_url(avatar_url, event->user);
    username_and_discriminator_to_str(username_and_discriminator, event->user);
    id_to_str(user_id_str, event->user->id);
    user_mention(user_mention_str, event->user->id);
    snprintf(timestamp_str, TIMESTAMP_NORMAL_STR_LEN, "<t:%lu>", cog_timestamp_ms()/1000);

    embed.color = COLOR_MINT;
    embed.timestamp = event->joined_at;

    discord_embed_set_author(&embed, event->user->username, NULL, avatar_url, NULL);
    discord_embed_set_thumbnail(&embed, avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);

    if (event->user->bot) {
        discord_add_guild_member_role(client, event->guild_id, event->user->id, R_BOT, &role_params, NULL);

        discord_embed_set_title(&embed, "New bot %s", username_and_discriminator);
        discord_embed_add_field(&embed, "Bot ID", user_id_str, true);
        discord_embed_add_field(&embed, "Bot", user_mention_str, true);
    }
    else {
        discord_add_guild_member_role(client, event->guild_id, event->user->id, R_SPECIAL, &role_params, NULL);
        discord_add_guild_member_role(client, event->guild_id, event->user->id, R_PRONOUNS, &role_params, NULL);
        discord_add_guild_member_role(client, event->guild_id, event->user->id, R_LANGS, &role_params, NULL);
        discord_add_guild_member_role(client, event->guild_id, event->user->id, R_LANS_HELP, &role_params, NULL);
        discord_add_guild_member_role(client, event->guild_id, event->user->id, R_FIELDS, &role_params, NULL);
        discord_add_guild_member_role(client, event->guild_id, event->user->id, R_FUN, &role_params, NULL);
        discord_add_guild_member_role(client, event->guild_id, event->user->id, R_OTHER, &role_params, NULL);

        discord_embed_set_title(&embed, "Welcome %s!", username_and_discriminator);
        discord_embed_set_description(&embed, "Welcome **%s** to PD! Please checkout <#%lu> and <#%lu> to get started!", user_mention_str, C_SERVER_INFO, C_ROLES);
        snprintf(footer_text, sizeof(footer_text), "ID: %lu", event->user->id);
        discord_embed_set_footer(&embed, footer_text, avatar_url, NULL);

        discord_create_message(client, C_WELCOME, &message_params, NULL);

        discord_embed_set_title(&embed, "New user %s", username_and_discriminator);
        discord_embed_set_description(&embed, "");
        discord_embed_add_field(&embed, "User ID", user_id_str, true);
        discord_embed_add_field(&embed, "User", user_mention_str, true);
    }

    discord_embed_add_field(&embed, "Joined at ", timestamp_str, true);

    discord_create_message(client, C_LOG, &message_params, NULL);

    free(avatar_url);
    free(user_mention_str);
    discord_embed_cleanup(&embed);

    return;
}
