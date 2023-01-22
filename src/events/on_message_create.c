#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_message_create(struct discord *client, const struct discord_message *message) {
    if (!message->author->bot) add_message_db(message);
    
    switch (message->channel_id) {
        case C_COUNTER: {
            struct discord_delete_message delete_params = {
            .reason = "Wrong number"
            };

            for (size_t i=0; i<strlen(message->content); i++) {
                if (message->content[i] == '\n') {
                    discord_delete_message(client, message->channel_id, message->id, &delete_params, NULL);
                    break;
                }
            }

            size_t val = atoi((char*)message->content);
            if (val) {
                struct discord_messages msgs = { 0 };
                struct discord_get_channel_messages params = {.limit = 2};
                struct discord_ret_messages ret_msgs = {.sync = &msgs};

                discord_get_channel_messages(client, message->channel_id, &params, &ret_msgs);
                if (msgs.size) {
                    size_t prev_val = atoi((char*)msgs.array[1].content);
                    if (val-1 == prev_val) break;
                    else discord_delete_message(client, message->channel_id, message->id, &delete_params, NULL);
                }
            }
            else discord_delete_message(client, message->channel_id, message->id, &delete_params, NULL);

            break;
        }
        case C_ONE_WORD: {
            struct discord_delete_message delete_params = {
                .reason = "Not one word"
            };

            for (size_t i=0; i<strlen(message->content); i++) {
                if (message->content[i] == ' ') discord_delete_message(client, message->channel_id, message->id, &delete_params, NULL);
                else continue;
            }

            break;
        }
        default: break;
    }
}
