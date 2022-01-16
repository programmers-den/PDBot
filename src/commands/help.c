#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

void help(struct discord *client, const struct discord_message *msg) {
    if (msg->author->bot) return;

    struct discord_embed *embed = load_embed_from_json(HELP_JSON);
    struct discord_create_message_params params = {.embed = embed};

    embed->author = &(struct discord_embed_author) {.name = msg->author->username};
    embed->timestamp = cog_timestamp_ms();
    
    discord_create_message(client, msg->channel_id, &params, NULL);

    free(embed);

    return;
}
