#include <concord/discord.h>
#include "../libs/bot_include.h"

void add_role_all_user(struct discord *client, const struct discord_interaction *interaction) {
    u64snowflake role_id;
    char *author_avatar_url = malloc(AVATAR_URL_LEN);
    char author_id_str[ID_STR_LEN+11];
    struct discord_guild guild = {.members = &(struct discord_guild_members) { 0 }};
    struct discord_embed embed;
    discord_guild_init(&guild);
    discord_embed_init(&embed);
    struct discord_interaction_response interaction_response = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data) {
            .embeds = &(struct discord_embeds) {
                .size = 1,
                .array = &embed
            }
        }
    };

    embed.color = COLOR_YELLOW;
    embed.timestamp = discord_timestamp(client);
    get_avatar_url(author_avatar_url, interaction->member->user);
    snprintf(author_id_str, sizeof(&author_id_str), "Author ID: %lu", interaction->member->user->id);
    discord_embed_set_footer(&embed, author_id_str, author_avatar_url, NULL);
    discord_embed_set_author(&embed, interaction->member->user->username, NULL, author_avatar_url, NULL);

    char role_mention_str[ROLE_MENTION_LEN];
    struct discord_edit_message edit = {
        .embeds = &(struct discord_embeds) {
            .size = 1,
            .array = &embed
        }
    };
    struct discord_list_guild_members list_guild_members = {.limit = 1000};
    struct discord_guild_preview guild_preview;
    struct discord_ret_guild ret_guild = {.sync = &guild};
    struct discord_ret_guild_members ret_guild_members = {.sync = guild.members};
    struct discord_ret_guild_preview ret_guild_preview = {.sync = &guild_preview};
    struct discord_edit_original_interaction_response original_interaction_response = {
        .embeds = &(struct discord_embeds) { 
            .size = 1,
            .array = &embed
        }
    };
    struct discord_add_guild_member_role params = {
        .reason = "PDBot command ran by a mod"
    };

    discord_get_guild(client, interaction->guild_id, &ret_guild);
    discord_list_guild_members(client, interaction->guild_id, &list_guild_members, &ret_guild_members);
    discord_get_guild_preview(client, interaction->guild_id, &ret_guild_preview);

    for (int i=0; i<interaction->data->options->size; i++) {
        if (!strcmp(interaction->data->options->array[i].name, "id")) {
            role_id = strtol(interaction->data->options->array[i].value, NULL, 10);
            role_mention(role_mention_str, role_id);

            discord_embed_set_title(&embed, "Please wait...");
            discord_embed_set_description(&embed, "Adding %s to %d memebers", role_mention_str, guild_preview.approximate_member_count);

            discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_response, NULL);

            for (size_t j=0; j<guild.members->size; j++) {
                if (!guild.members->array[j].user->bot) {
                    discord_add_guild_member_role(client, interaction->guild_id, guild.members->array[j].user->id, role_id, &params, NULL);
                }
            }
        }
    }

    embed.color = COLOR_MINT;
    discord_embed_set_title(&embed, "Done! Took %lu ms", (discord_timestamp(client)-embed.timestamp)/1000);
    discord_embed_set_description(&embed, "Added %s to %d members", role_mention_str, guild_preview.approximate_member_count);

    discord_edit_original_interaction_response(client, ID_APPLICATION, interaction->token, &original_interaction_response, NULL);

    free(author_avatar_url);
    discord_guild_cleanup(&guild);
    discord_embed_cleanup(&embed);

    return;
}
