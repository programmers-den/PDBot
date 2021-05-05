#include <orca/discord.h>
#include <orca/orka-utils.h>

void help(struct discord *client, const struct discord_user *user, const struct discord_message *msg) {
    if (msg->author->bot) return;

    struct discord_embed *embed = discord_embed_alloc();
    strcpy(embed->title, "[Comamnds]");
    strcpy(embed->description, "help\nping");
    embed->author = msg->author;
    embed->timestamp = orka_timestamp_ms();
    embed->color = 49151;
    struct discord_create_message_params params = {.embed = embed};
    discord_create_message(client, msg->channel_id, &params, NULL);
}
