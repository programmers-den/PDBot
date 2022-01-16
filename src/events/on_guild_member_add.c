#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_guild_member_add(struct discord *client, const u64_snowflake_t guild_id, const struct discord_guild_member *member) {
    char username_and_discriminator[DISCORD_MAX_USERNAME_LEN], user_id_str[ID_STR_LEN], timestamp_str[TIMESTAMP_NORMAL_STR_LEN];
    char *avatar_url = malloc(AVATAR_URL_LEN), *user_mention_str = malloc(USER_MENTION_LEN);
    struct discord_embed embed;
    discord_embed_init(&embed);
    struct discord_create_message_params params = {.embed = &embed};

    get_avatar_url(avatar_url, member->user);
    username_and_discriminator_to_str(username_and_discriminator, member->user);
    id_to_str(user_id_str, member->user->id);
    user_mention(user_mention_str, member->user->id);
    snprintf(timestamp_str, TIMESTAMP_NORMAL_STR_LEN, "<t:%lu>", cog_timestamp_ms()/1000);

    embed.color = COLOR_MINT;
    embed.timestamp = member->joined_at;

    discord_embed_set_author(&embed, member->user->username, NULL, avatar_url, NULL);
    discord_embed_set_thumbnail(&embed, avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);

    if (member->user->bot) {
        discord_add_guild_member_role(client, guild_id, member->user->id, R_BOT);

        snprintf(embed.title, sizeof(embed.title), "New bot %s", username_and_discriminator);
        discord_embed_add_field(&embed, "Bot ID", user_id_str, true);
        discord_embed_add_field(&embed, "Bot", user_mention_str, true);
    }
    else {
        discord_add_guild_member_role(client, guild_id, member->user->id, R_SPECIAL);
        discord_add_guild_member_role(client, guild_id, member->user->id, R_PRONOUNS);
        discord_add_guild_member_role(client, guild_id, member->user->id, R_LANGS);
        discord_add_guild_member_role(client, guild_id, member->user->id, R_LANS_HELP);
        discord_add_guild_member_role(client, guild_id, member->user->id, R_FIELDS);
        discord_add_guild_member_role(client, guild_id, member->user->id, R_FUN);
        discord_add_guild_member_role(client, guild_id, member->user->id, R_OTHER);

        snprintf(embed.title, sizeof(embed.title), "Welcome %s!", username_and_discriminator);
        snprintf(embed.description, sizeof(embed.description), "Welcome **%s** to PD! Please checkout <#%lu> and <#%lu> to get started!", user_mention_str, C_SERVER_INFO, C_ROLES);
        snprintf(embed.footer->text, sizeof(embed.footer->text), "ID: %lu", member->user->id);

        discord_create_message(client, C_WELCOME, &params, NULL);

        snprintf(embed.title, sizeof(embed.title), "New user %s", username_and_discriminator);
        snprintf(embed.description, 1, "");
        discord_embed_add_field(&embed, "User ID", user_id_str, true);
        discord_embed_add_field(&embed, "User", user_mention_str, true);
    }

    discord_embed_add_field(&embed, "Joined at ", timestamp_str, true);

    discord_create_message(client, C_LOG, &params, NULL);

    free(avatar_url);
    free(user_mention_str);
    discord_embed_cleanup(&embed);

    return;
}
