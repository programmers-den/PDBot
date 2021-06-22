#include <orca/discord.h>
#include "../libs/bot_include.h"

void add_role_all_user(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {
    if (msg->author->bot) return;

    char author_avatar_url[AVATAR_URL_LEN];
    struct discord_guild_member *guild_member = discord_guild_member_alloc();
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_guild *guild = discord_guild_alloc();

    embed->timestamp = msg->timestamp;
    get_avatar_url(author_avatar_url, msg->author);
    snprintf(embed->footer->text, 2049, "Author ID: %lu", msg->author->id);
    discord_embed_set_author(embed, msg->author->username, NULL, author_avatar_url, NULL);
    discord_get_guild_member(client, msg->guild_id, msg->author->id, guild_member);

    if (!guild_member_has_role(guild_member, R_OWNER)) {
        char role_mention_str[ROLE_MENTION_LEN];
        struct discord_create_message_params params = {.embed = embed};
        role_mention(role_mention_str, R_OWNER);
        embed->color = COLOR_RED;
        snprintf(embed->title, 257, "No permission!");
        discord_embed_add_field(embed, "Required role", role_mention_str, true);

        discord_create_message(client, msg->channel_id, &params, NULL);

        discord_guild_member_free(guild_member);
        discord_embed_free(embed);

        return;
    }
    else {
        discord_guild_member_free(guild_member);
        size_t err_arg = 0;

        if ((err_arg = get_args_ids_as_int(NULL, msg, " "))) {
            char arg_str[MAX_CMD_ARGS_LEN];
            struct discord_create_message_params params = {.embed = embed};
            get_arg_at(arg_str, msg, err_arg, " ");

            embed->color = COLOR_RED;
            snprintf(embed->title, 257, "Not an id!");
            snprintf(embed->description, 2049, "Error arg[%lu]: %s", err_arg, arg_str[0]?arg_str:"NULL");

            discord_create_message(client, msg->channel_id, &params, NULL);

            discord_guild_member_free(guild_member);
            discord_embed_free(embed);

            return;
        }
        else {
            size_t args_count = get_args_len(msg, " ");
            u64_snowflake_t args_ids_int[args_count];
            char args_id_str[ID_STR_LEN], role_mention_str[ROLE_MENTION_LEN];
            // struct discord_guild *guild = discord_guild_alloc(); had to move to line 10 to prevent dbus
            struct discord_message *embed_message = NULL;
            struct discord_create_message_params params = {.embed = embed};
            struct discord_edit_message_params edit_params = {.embed = embed};
            struct discord_list_guild_members_params list_guild_members_params = {.limit = 1000};
            struct discord_guild_preview guild_preview;

            // deletes client
            get_args_ids_as_int(args_ids_int, msg, " ");
            discord_get_guild(client, msg->guild_id, guild);
            discord_list_guild_members(client, msg->guild_id, &list_guild_members_params, &guild->members);
            discord_get_guild_preview(client, msg->guild_id, &guild_preview);

            embed->color = COLOR_YELLOW;
            snprintf(embed->title, 257, "Please wait...");
            snprintf(embed->description, 2049, "Adding roles to %d members", guild_preview.approximate_member_count);

            for (size_t i=0; i<=args_count; i++) {
                printf("i: %lu : %lu", i, args_ids_int[i]);
                role_mention(role_mention_str, args_ids_int[i]);

                discord_embed_add_field(embed, args_id_str, role_mention_str, false);
            }

            discord_create_message(client, msg->channel_id, &params, embed_message);

            for (size_t i=0; args_ids_int[i]; i++) {
                for (size_t j=0; guild->members[j]; j++) {
                    if (!guild->members[j]->user->bot) {
                        discord_add_guild_member_role(client, msg->guild_id, guild->members[j]->user->id, args_ids_int[i]);
                    }
                }
            }

            embed->color = COLOR_MINT;
            snprintf(embed->title, 257, "Done! Took %lu ms", cee_timestamp_ms()-msg->timestamp);
            snprintf(embed->description, 2049, "Added roles to %d members", guild_preview.approximate_member_count);

            discord_edit_message(client, msg->channel_id, embed_message->id, &edit_params, NULL);

            discord_guild_free(guild);
            discord_embed_free(embed);
            discord_message_free(embed_message);
        }
    }

    return;
}
