#include <concord/discord.h>
#include "../libs/bot_include.h"

void get_message_url(char buf[], const struct discord_message *message) {
    // buf[MESSAGE_URL_LEN]
    memset(buf, 0, MESSAGE_URL_LEN);
    snprintf(buf, MESSAGE_URL_LEN, "https://discord.com/channels/%lu/%lu/%lu", message->guild_id, message->channel_id, message->id);

    return;
}
