#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void help(struct discord *client, const struct discord_interaction *interaction) {
    struct discord_embed *embed = load_embed_from_json(HELP_JSON);
    struct discord_interaction_response interaction_response = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data) {
            .embeds = &(struct discord_embeds) {
                .size = 1,
                .array = embed
            }
        }
    };

    embed->author = &(struct discord_embed_author) {.name = interaction->member->user->username};
    embed->timestamp = cog_timestamp_ms();

    discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_response, NULL);

    free(embed);

    return;
}
