#include <orca/discord.h>
#include "../libs/bot_include.h"

void on_message_create(struct discord *client, const struct discord_user *bot, const struct discord_message *message) {
    if (message->author->bot) return;

    add_message_db(message);
    switch (message->channel_id) {
        case C_VERIFY: {
            if (message->author->id != ID_OWNER) {
                cee_sleep_ms(VERIFY_SLEEP);
                discord_delete_message(client, message->channel_id, message->id);
            }

            break;
        }
        case C_COUNTER: {
            size_t val = atoi((char*)message->content);
            if (val) {
                NTL_T(struct discord_message) msgs = NULL;
                struct discord_get_channel_messages_params params = {.limit = 2};
                discord_get_channel_messages(client, message->channel_id, &params, &msgs);
                if (msgs) {
                    size_t prev_val = atoi((char*)msgs[1]->content);
                    if (val-1 == prev_val) break;
                    else discord_delete_message(client, message->channel_id, message->id);
                }
            }
            else discord_delete_message(client, message->channel_id, message->id);

            break;
        }
        case C_ONE_WORD: {
            for (size_t i=0; i<strlen(message->content); i++) {
                if (message->content[i] == ' ') discord_delete_message(client, message->channel_id, message->id);
                else continue;
            }

            break;
        }
        case C_POLL: {
            if (message->author->bot) break;

            char *avatar_url = malloc(AVATAR_URL_LEN);
            struct discord_embed *embed = discord_embed_alloc();
            struct discord_create_message_params params = {.embed = embed};
            struct discord_message *poll_message = discord_message_alloc();

            get_avatar_url(avatar_url, message->author);

            embed->color = COLOR_MAGENTA;
            discord_embed_set_author(embed, message->content, NULL, avatar_url, NULL);
            snprintf(embed->description, 2049, "<:%s:%lu> Yes\n<:%s:%lu> No", E_YES_NAME, E_YES_ID, E_NO_NAME, E_NO_ID);
            discord_create_message(client, message->channel_id, &params, poll_message);
            discord_delete_message(client, message->channel_id, message->id);
            discord_create_reaction(client, poll_message->channel_id, poll_message->id, E_YES_ID, E_YES_NAME);
            discord_create_reaction(client, poll_message->channel_id, poll_message->id, E_NO_ID, E_NO_NAME);

            free(avatar_url);
            discord_message_free(poll_message);
            discord_embed_free(embed);

            break;
        }
        default: break;
    }
}
