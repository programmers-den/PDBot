#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <orca/discord.h>
#include <orca/log.h>
#include "../libs/config.h"

void on_message_create(struct discord *client, const struct discord_user *bot, const struct discord_message *message) {
    switch (message->channel_id) {
        case C_COUNTER: {
            size_t val = atoi((char*)message->content);
            if (val) {
                NTL_T(struct discord_message) msgs = NULL;
                struct discord_get_channel_messages_params params = {.limit = 2};
                discord_get_channel_messages(client, message->channel_id, &params, &msgs);
                if (msgs) {
                    // No nullbyte
                    size_t prev_val = atoi((char*)msgs[1]->content);
                    if (val-1 == prev_val) break;
                    else discord_delete_message(client, message->channel_id, message->id);
                }
            }
            else discord_delete_message(client, message->channel_id, message->id);
            break;
        }
        default: break;
    }
}
