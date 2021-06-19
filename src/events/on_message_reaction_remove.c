#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

void on_message_reaction_remove(struct discord *client, const struct discord_user *bot, const u64_snowflake_t user_id, const u64_snowflake_t channel_id, const u64_snowflake_t message_id, const u64_snowflake_t guild_id, const struct discord_emoji *emoji) {
    struct discord_user *member = discord_user_alloc();
    discord_get_user(client, user_id, member);
    if (member->bot) return;

    size_t count = 0;
    char *icon_url = malloc(AVATAR_URL_LEN), message_id_str[ID_STR_LEN], channel_id_str[ID_STR_LEN], channel_str[CHANNEL_MENTiON_LEN], author_id_str[ID_STR_LEN], author_str[USER_MENTION_LEN], username_and_discriminator[USER_AND_DESCRIM_LEN], emoji_str[emoji_mention_len(emoji)];
    struct discord_message *message = discord_message_alloc();
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    message->guild_id = guild_id;
    embed->timestamp = cee_timestamp_ms();
    embed->color = COLOR_PINK_PANTONE;

    get_icon_url(icon_url, member);
    id_to_str(message_id_str, message_id);
    id_to_str(channel_id_str, channel_id);
    channel_mention(channel_str, channel_id);
    id_to_str(author_id_str, member->id);
    user_mention(author_str, member->id);
    username_and_discriminator_to_str(username_and_discriminator, member);
    emoji_mention(emoji_str, emoji);

    snprintf(embed->title, 257, "Reaction removed by %s %s", username_and_discriminator, emoji_str);

    discord_get_channel_message(client, channel_id, message_id, message);
    discord_embed_set_author(embed, member->username, NULL, icon_url, NULL);
    discord_embed_set_thumbnail(embed, icon_url, NULL, AVATAR_HEIGHT, AVATAR_WIDTH);
    discord_embed_add_field(embed, "Message ID", message_id_str, true);
    discord_embed_add_field(embed, "Channel ID", channel_id_str, true);
    discord_embed_add_field(embed, "Author ID", author_id_str, true);
    discord_embed_add_field(embed, "Channel", channel_str, false);
    discord_embed_add_field(embed, "Author", author_str, true);
    discord_embed_add_field(embed, "Content", message->content, false);
    snprintf(embed->footer->text, 2049, "Author ID: %lu", member->id);

    discord_create_message(client, C_LOG, &params, NULL);

    free(icon_url);
    discord_embed_free(embed);
    discord_message_free(message);

    return;
}
