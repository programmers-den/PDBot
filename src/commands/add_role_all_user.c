#include <concord/discord.h>
#include "../libs/bot_include.h"

void add_role_all_user(struct discord *client, const struct discord_interaction *interaction) {
    char *author_avatar_url = malloc(AVATAR_URL_LEN), *owner_role_mention = malloc(ROLE_MENTION_LEN);
    char author_id_str[ID_STR_LEN+11];
    struct discord_guild guild;
    struct discord_guild_member guild_member;
    struct discord_embed embed;
    discord_guild_init(&guild);
    discord_guild_member_init(&guild_member);
    discord_embed_init(&embed);
    struct discord_interaction_response interaction_params = {
        .type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data) {.embeds = (struct discord_embed *[]) {&embed, NULL}},
    };

    embed.timestamp = discord_timestamp(client);
    role_mention(owner_role_mention, R_OWNER);
    get_avatar_url(author_avatar_url, interaction->member->user);
    snprintf(author_id_str, sizeof(&author_id_str), "Author ID: %lu", interaction->member->user->id);
    discord_embed_set_footer(&embed, author_id_str, author_avatar_url, NULL);
    discord_embed_set_author(&embed, interaction->member->user->username, NULL, author_avatar_url, NULL);
    discord_get_guild_member(client, interaction->guild_id, interaction->member->user->id, &guild_member);

    if (!guild_member_has_role(&guild_member, R_OWNER)) {
        embed.color = COLOR_RED;
        discord_embed_set_title(&embed, "No permission!");
        discord_embed_add_field(&embed, "Required role", owner_role_mention, true);

        discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_params, NULL);

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
        discord_guild_cleanup(&guild);

        u64_snowflake_t role_id;
        char role_mention_str[ROLE_MENTION_LEN];
        struct discord_edit_message_params edit_params = {.embed = &embed};
        struct discord_list_guild_members_params list_guild_members_params = {.limit = 1000};
        struct discord_guild_preview guild_preview;
        struct discord_edit_original_interaction_response_params interaction_response_params = {
            .embeds = (struct discord_embed *[]) {&embed, NULL}
        };

        embed.color = COLOR_YELLOW;

        discord_get_guild(client, interaction->guild_id, &guild);
        discord_list_guild_members(client, interaction->guild_id, &list_guild_members_params, &guild.members);
        discord_get_guild_preview(client, interaction->guild_id, &guild_preview);

        for (int i=0; interaction->data->options[i]; i++) {
            if (!strcmp(interaction->data->options[i]->name, "id")) {
                role_id = strtol(interaction->data->options[i]->value, NULL, 10);
                role_mention(role_mention_str, role_id);

                discord_embed_set_title(&embed, "Please wait...");
                discord_embed_set_description(&embed, "Adding %s to %d memebers", role_mention_str, guild_preview.approximate_member_count);

                discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_params, NULL);

                for (size_t j=0; guild.members[j]; j++) {
                    if (!guild.members[j]->user->bot) {
                        discord_add_guild_member_role(client, interaction->guild_id, guild.members[j]->user->id, role_id);
                    }
                }
            }
        }

        embed.color = COLOR_MINT;
        discord_embed_set_title(&embed, "Done! Took %lu ms", (discord_timestamp(client)-embed.timestamp)/1000);
        discord_embed_set_description(&embed, "Added %s to %d members", role_mention_str, guild_preview.approximate_member_count);

        discord_edit_original_interaction_response(client, ID_APPLICATION, interaction->token, &interaction_response_params, NULL);

        discord_guild_cleanup(&guild);
        discord_embed_cleanup(&embed);

        return;
    }

    return;
}
