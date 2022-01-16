#include <concord/discord.h>
#include "../libs/bot_include.h"

void update(struct discord *client, const struct discord_message *msg) {
    if (msg->author->bot) return;

    char *author_avatar_url = malloc(AVATAR_URL_LEN), *owner_role_mention = malloc(ROLE_MENTION_LEN);
    struct discord_guild guild;
    struct discord_guild_member guild_member;
    struct discord_embed embed;
    discord_guild_init(&guild);
    discord_guild_member_init(&guild_member);
    discord_embed_init(&embed);
    struct discord_create_message_params params = {.embed = &embed};

    embed.timestamp = msg->timestamp;
    role_mention(owner_role_mention, R_OWNER);
    get_avatar_url(author_avatar_url, msg->author);
    snprintf(embed.footer->text, sizeof(embed.footer->text), "Author ID: %lu", msg->author->id);
    discord_embed_set_author(&embed, msg->author->username, NULL, author_avatar_url, NULL);
    discord_get_guild_member(client, msg->guild_id, msg->author->id, &guild_member);

    if (!guild_member_has_role(&guild_member, R_OWNER)) {
        embed.color = COLOR_RED;
        snprintf(embed.title, sizeof(embed.title), "No permission!");
        discord_embed_add_field(&embed, "Required role", owner_role_mention, true);

        discord_create_message(client, msg->channel_id, &params, NULL);

        free(author_avatar_url);
        free(owner_role_mention);
        discord_guild_member_cleanup(&guild_member);
        discord_guild_cleanup(&guild);
        discord_embed_cleanup(&embed);
    }
    else {
        embed.color = COLOR_MINT;
        snprintf(embed.title, sizeof(embed.title), "Updating...");
        snprintf(embed.description, sizeof(embed.description), "Please wait a brief moment...");

        discord_create_message(client, msg->channel_id, &params, NULL);

        system("git pull");

        free(author_avatar_url);
        free(owner_role_mention);
        discord_guild_member_cleanup(&guild_member);
        discord_guild_cleanup(&guild);
        discord_embed_cleanup(&embed);
        discord_cleanup(client);
        ccord_global_cleanup();
    }

    return;
}
