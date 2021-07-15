#include <orca/discord.h>
#include "../libs/bot_include.h"

void on_message_create(struct discord *client, const struct discord_user *bot, const struct discord_message *message) {
    if (!message->author->bot) add_message_db(message);

    switch (message->channel_id) {
        case C_VERIFY: {
            if (message->author->id != ID_OWNER) {
                cee_sleep_ms(VERIFY_SLEEP);
                discord_delete_message(client, message->channel_id, message->id);
            }

            break;
        }
        case C_COUNTER: {
            for (size_t i=0; i<strlen(message->content); i++) {
                if (message->content[i] == '\n') {
                    discord_delete_message(client, message->channel_id, message->id);
                    break;
                }
            }

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

            char author_str[USER_AND_DESCRIM_LEN];
            char *avatar_url = malloc(AVATAR_URL_LEN);
            struct discord_emoji *emoji_yes = discord_emoji_alloc();
            struct discord_emoji *emoji_no = discord_emoji_alloc();
            struct discord_embed *embed = discord_embed_alloc();
            struct discord_create_message_params params = {.embed = embed};
            struct discord_message *poll_message = discord_message_alloc();

            discord_get_guild_emoji(client, message->guild_id, E_YES_ID, emoji_yes);
            discord_get_guild_emoji(client, message->guild_id, E_NO_ID, emoji_no);
            char emoji_yes_str[emoji_mention_len(emoji_yes)], emoji_no_str[emoji_mention_len(emoji_no)];

            username_and_discriminator_to_str(&author_str, message->author);
            get_avatar_url(avatar_url, message->author);
            emoji_mention(&emoji_yes_str, emoji_yes);
            emoji_mention(&emoji_no_str, emoji_no);

            embed->color = COLOR_MAGENTA;
            embed->timestamp = message->timestamp;
            discord_embed_set_author(embed, message->content, message->author->username, avatar_url, NULL);
            snprintf(embed->title, sizeof(embed->title), avatar_url);
            discord_embed_add_field(embed, "Yes", emoji_yes_str, true);
            discord_embed_add_field(embed, "No", emoji_no_str, true);
            snprintf(embed->description, sizeof(embed->description), author_str);
            discord_create_message(client, message->channel_id, &params, poll_message);
            discord_delete_message(client, message->channel_id, message->id);
            discord_create_reaction(client, poll_message->channel_id, poll_message->id, E_YES_ID, E_YES_NAME);
            discord_create_reaction(client, poll_message->channel_id, poll_message->id, E_NO_ID, E_NO_NAME);

            free(avatar_url);
            discord_emoji_free(emoji_yes);
            discord_emoji_free(emoji_no);
            discord_embed_free(embed);
            discord_message_free(poll_message);

            break;
        }
        default: break;
    }
}
