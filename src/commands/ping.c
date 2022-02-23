#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void ping(struct discord *client, const struct discord_interaction *interaction) {
    char content[TIMESTAMP_STR_LEN+10];
    struct discord_interaction_response interaction_response = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data) {.content = content} 
    };

    snprintf(content, TIMESTAMP_STR_LEN+10, "Pong! %dms", discord_get_ping(client));

    discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_response, NULL);
}
