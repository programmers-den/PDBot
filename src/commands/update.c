#include <concord/discord.h>
#include "../libs/bot_include.h"

void update(struct discord *client, const struct discord_interaction *interaction) {
    char *author_avatar_url = malloc(AVATAR_URL_LEN), *owner_role_mention = malloc(ROLE_MENTION_LEN);
    char author_id_str[ID_STR_LEN+11];
    struct discord_guild guild;
    struct discord_guild_member guild_member;
    struct discord_embed embed;
    discord_guild_init(&guild);
    discord_guild_member_init(&guild_member);
    discord_embed_init(&embed);
    struct discord_create_message params = {.embed = &embed};
    struct discord_interaction_response interaction = {
	    .type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE,
	    .data = &(struct discord_interaction_callback_data) {.embeds = (struct discord_embed *[]) {&embed, NULL}}
    };

    embed.timestamp = discord_timestamp(client);;
    role_mention(owner_role_mention, R_OWNER);
    get_avatar_url(author_avatar_url, interaction->member->user);
    
    snprintf(author_id_str, sizeof(author_id_str), "Author ID: %lu", interaction->member->user->id);
    discord_embed_set_footer(&embed, author_id_str, author_avatar_url, NULL);
    discord_embed_set_author(&embed, interaction->member->user->username, NULL, author_avatar_url, NULL);
    discord_get_guild_member(client, interaction->guild_id, interaction->member->user->id, &guild_member);

    if (!guild_member_has_role(&guild_member, R_OWNER)) {
        embed.color = COLOR_RED;
	discord_embed_set_title(&embed, "No permission");
        discord_embed_add_field(&embed, "Required role", owner_role_mention, true);

    	discord_create_interaction_response(client, interaction->id, interaction->token, &interaction, NULL);

        free(author_avatar_url);
        free(owner_role_mention);
        discord_guild_member_cleanup(&guild_member);
        discord_guild_cleanup(&guild);
        discord_embed_cleanup(&embed);
    }
    else {
        embed.color = COLOR_MINT;
        discord_embed_set_title(&embed, "Updating...");
	discord_embed_set_description(&embed, "Please wait a brief moment...");

    	discord_create_interaction_response(client, interaction->id, interaction->token, &interaction, NULL);

        system("git pull && make");

        free(author_avatar_url);
        free(owner_role_mention);
        discord_guild_member_cleanup(&guild_member);
        discord_guild_cleanup(&guild);
        discord_embed_cleanup(&embed);
        discord_shutdown(client);
    }

    return;
}
