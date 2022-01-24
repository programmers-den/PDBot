#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void stats(struct discord *client, const struct discord_interaction *interaction) {
    char user_id[ID_STR_LEN];
    char *author_avatar_url = malloc(AVATAR_URL_LEN);
    struct discord_embed embed;
    struct discord_interaction_response interaction_response = {
        .type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data) {.embeds = (struct discord_embed *[]) {&embed, NULL}}
    };

    get_avatar_url(author_avatar_url, interaction->member->user);
    snprintf(user_id, ID_STR_LEN, "%lu", interaction->member->user->id);

    discord_embed_init(&embed);
    embed.timestamp = discord_timestamp(client);
    discord_embed_set_author(&embed, interaction->member->user->username, NULL, author_avatar_url, NULL);
    discord_embed_set_image(&embed, author_avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    discord_embed_add_field(&embed, "ID", user_id, false);

    discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_response, NULL);

    free(author_avatar_url);
    discord_embed_cleanup(&embed);

    return;
}
