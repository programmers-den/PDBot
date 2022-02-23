#include <concord/discord.h>
#include "../libs/bot_include.h"

void update(struct discord *client, const struct discord_interaction *interaction) {
    char *author_avatar_url = malloc(AVATAR_URL_LEN);
    char author_id_str[ID_STR_LEN+11];
    struct discord_embed embed;
    discord_embed_init(&embed);
    struct discord_create_message params = {
        .embeds = &(struct discord_embeds) {
            .size = 1,
            .array = &embed
        }
    };
    struct discord_interaction_response interaction_response = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data) {
            .embeds = &(struct discord_embeds) {
                .size = 1,
                .array = &embed
            }
        }
    };

    embed.color = COLOR_MINT;
    embed.timestamp = discord_timestamp(client);;
    get_avatar_url(author_avatar_url, interaction->member->user);

    snprintf(author_id_str, sizeof(author_id_str), "Author ID: %lu", interaction->member->user->id);
    discord_embed_set_footer(&embed, author_id_str, author_avatar_url, NULL);
    discord_embed_set_author(&embed, interaction->member->user->username, NULL, author_avatar_url, NULL);
    discord_embed_set_title(&embed, "Updating...");
    discord_embed_set_description(&embed, "Please wait a brief moment...");

    discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_response, NULL);

    system("git pull && make");

    free(author_avatar_url);
    discord_embed_cleanup(&embed);
    close_db();
    discord_shutdown(client);

    return;
}
