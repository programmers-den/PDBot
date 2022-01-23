#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void icon(struct discord *client, const struct discord_interaction *interaction) {
    struct discord_embed embed;
    struct discord_interaction_response interaction = {
	    .type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE,
	    .data = &(struct discord_interaction_callback_data) {.embeds = (struct discord_embed *[]) {&embed, NULL}}
    };

    discord_embed_init(&embed);
    char *author_avatar_url = malloc(AVATAR_URL_LEN);

    embed.timestamp = cog_timestamp_ms();
    embed.color = COLOR_AERO;

    get_avatar_url(author_avatar_url, interaction->member->user);
    discord_embed_set_author(&embed, interaction->member->user->username, NULL, author_avatar_url, NULL);
    discord_embed_set_image(&embed, author_avatar_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);

    discord_create_interaction_response(client, interaction->id, interaction->token, &interaction, NULL);
 
    free(author_avatar_url);
    discord_embed_cleanup(&embed);

    return;
}
