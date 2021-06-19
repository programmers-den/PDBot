#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

char *get_message_url(const struct discord_message *message) {
    char *url = malloc(MESSAGE_URL_LENGTH);
    snprintf(url, MESSAGE_URL_LENGTH, "https://discord.com/channels/%lu/%lu/%lu", message->guild_id, message->channel_id, message->id);

    return url;
}
