#include <signal.h>
#include <sqlite3.h>
#include <concord/discord.h>
#include "src/libs/bot_include.h"

sqlite3 *db = NULL;
FILE *dbFile = NULL;
int dbIsOpen = 0;
struct discord *client = NULL;

int main() {
    ccord_global_init();
    client = discord_config_init("bot.config");

    signal(SIGINT, &on_sigint);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_message_create(client, &on_message_create);
    discord_set_on_message_delete(client, &on_message_delete);
    discord_set_on_message_update(client, &on_message_update);
    discord_set_on_message_reaction_add(client, &on_message_reaction_add);
    discord_set_on_message_reaction_remove(client, &on_message_reaction_remove);
    discord_set_on_guild_member_add(client, &on_guild_member_add);
    discord_set_on_guild_member_remove(client, &on_guild_member_remove);
    discord_set_on_interaction_create(client, &on_interaction_create);
    discord_set_on_channel_create(client, &on_channel_create);
    discord_set_on_channel_delete(client, &on_channel_delete);
    discord_set_on_channel_update(client, &on_channel_update);

    discord_run(client);

    discord_cleanup(client);
    ccord_global_cleanup();

    return 0;
}
