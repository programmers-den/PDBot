#include <orca/discord.h>
#include "../libs/bot_include.h"

void rm_role_all_user(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {
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

        return;
    }
    else {
        size_t err_arg = 0;

        if ((err_arg = check_if_args_ids(msg, " "))) {
            char *arg_str = malloc(MAX_CMD_ARGS_LEN);
            struct discord_create_message_params params = {.embed = &embed};
            get_arg_at(arg_str, msg, err_arg, " ");

            embed.color = COLOR_RED;
            snprintf(embed.title, sizeof(embed.title), "Not an id!");
            snprintf(embed.description, sizeof(embed.description), "Error arg[%zu]: %s", err_arg, arg_str[0]?arg_str:"NULL");

            discord_create_message(client, msg->channel_id, &params, NULL);

            free(arg_str);
            free(author_avatar_url);
            free(owner_role_mention);
            discord_guild_member_cleanup(&guild_member);
            discord_guild_cleanup(&guild);
            discord_embed_cleanup(&embed);

            return;
        }
        else {
            free(author_avatar_url);
            free(owner_role_mention);
            discord_guild_member_cleanup(&guild_member);

            u64_snowflake_t arg_id_int = get_args_ids_as_int_at(msg, 1, " ");
            char role_mention_str[ROLE_MENTION_LEN];
            struct discord_message embed_message;
            discord_message_init(&embed_message);
            struct discord_edit_message_params edit_params = {.embed = &embed};
            struct discord_list_guild_members_params list_guild_members_params = {.limit = 1000};
            struct discord_guild_preview guild_preview;
            embed.color = COLOR_YELLOW;

            discord_get_guild(client, msg->guild_id, &guild);
            discord_list_guild_members(client, msg->guild_id, &list_guild_members_params, &guild.members);
            discord_get_guild_preview(client, msg->guild_id, &guild_preview);
            role_mention(role_mention_str, arg_id_int);

            snprintf(embed.title, sizeof(embed.title), "Please wait...");
            snprintf(embed.description, sizeof(embed.description), "Removing %s from %d members", role_mention_str, guild_preview.approximate_member_count);

            discord_create_message(client, msg->channel_id, &params, &embed_message);

            for (size_t j=0; guild.members[j]; j++) {
                if (!guild.members[j]->user->bot) {
                    discord_remove_guild_member_role(client, msg->guild_id, guild.members[j]->user->id, arg_id_int);
                }
            }

            embed.color = COLOR_PINK_PANTONE;
            snprintf(embed.title, sizeof(embed.title), "Done! Took %lu ms", cee_timestamp_ms()-msg->timestamp);
            snprintf(embed.description, sizeof(embed.description), "Removed %s from %d members", role_mention_str, guild_preview.approximate_member_count);

            discord_edit_message(client, msg->channel_id, embed_message.id, &edit_params, NULL);

            discord_guild_cleanup(&guild);
            discord_embed_cleanup(&embed);
            discord_message_cleanup(&embed_message);

            return;
        }
    }

    return;
}
