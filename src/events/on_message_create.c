#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_message_create(struct discord *client, const struct discord_message *message) {
    if (!message->author->bot) add_message_db(message);

    switch (message->channel_id) {
        case C_COUNTER: {
                            for (size_t i=0; i<strlen(message->content); i++) {
                                if (message->content[i] == '\n') {
                                    discord_delete_message(client, message->channel_id, message->id, NULL);
                                    break;
                                }
                            }

                            size_t val = atoi((char*)message->content);
                            if (val) {
                                NTL_T(struct discord_message) msgs = NULL;
                                struct discord_get_channel_messages params = {.limit = 2};
                                struct discord_ret_messages ret_msgs = {.sync = &msgs};

                                discord_get_channel_messages(client, message->channel_id, &params, &ret_msgs);
                                if (msgs) {
                                    size_t prev_val = atoi((char*)msgs[1]->content);
                                    if (val-1 == prev_val) break;
                                    else discord_delete_message(client, message->channel_id, message->id, NULL);
                                }
                            }
                            else discord_delete_message(client, message->channel_id, message->id, NULL);

                            break;
                        }
        case C_ONE_WORD: {
                             for (size_t i=0; i<strlen(message->content); i++) {
                                 if (message->content[i] == ' ') discord_delete_message(client, message->channel_id, message->id, NULL);
                                 else continue;
                             }

                             break;
                         }
        case C_POLL: {
                         if (message->author->bot) break;

                         char author_str[USER_AND_DESCRIM_LEN];
                         char *avatar_url = malloc(AVATAR_URL_LEN);
                         struct discord_emoji emoji_yes;
                         struct discord_emoji emoji_no;
                         struct discord_embed embed;
                         struct discord_create_message params = {.embed = &embed};
                         struct discord_message poll_message;
                         struct discord_ret_emoji ret_emoji_yes = {.sync = &emoji_yes};
                         struct discord_ret_emoji ret_emoji_no = {.sync = &emoji_no};
                         struct discord_ret_message ret_poll_message = {.sync = &poll_message};
                         discord_emoji_init(&emoji_yes);
                         discord_emoji_init(&emoji_no);
                         discord_embed_init(&embed);
                         discord_message_init(&poll_message);

                         discord_get_guild_emoji(client, message->guild_id, E_YES_ID, &ret_emoji_yes);
                         discord_get_guild_emoji(client, message->guild_id, E_NO_ID, &ret_emoji_no);
                         char emoji_yes_str[emoji_mention_len(&emoji_yes)], emoji_no_str[emoji_mention_len(&emoji_no)];

                         username_and_discriminator_to_str((char*)&author_str, message->author);
                         get_avatar_url(avatar_url, message->author);
                         emoji_mention((char*)&emoji_yes_str, &emoji_yes);
                         emoji_mention((char*)&emoji_no_str, &emoji_no);

                         embed.color = COLOR_MAGENTA;
                         embed.timestamp = message->timestamp;
                         discord_embed_set_author(&embed, author_str, NULL, avatar_url, NULL);
                         snprintf(embed.description, sizeof(embed.description), "%s", message->content);
                         discord_embed_add_field(&embed, "Yes", emoji_yes_str, true);
                         discord_embed_add_field(&embed, "No", emoji_no_str, true);
                         discord_create_message(client, message->channel_id, &params, &ret_poll_message);
                         discord_delete_message(client, message->channel_id, message->id, NULL);
                         discord_create_reaction(client, poll_message.channel_id, poll_message.id, E_YES_ID, E_YES_NAME, NULL);
                         cog_sleep_ms(100);
                         discord_create_reaction(client, poll_message.channel_id, poll_message.id, E_NO_ID, E_NO_NAME, NULL);

                         free(avatar_url);
                         discord_emoji_cleanup(&emoji_yes);
                         discord_emoji_cleanup(&emoji_no);
                         discord_embed_cleanup(&embed);
                         discord_message_cleanup(&poll_message);

                         break;
                     }
        default: break;
    }
}
