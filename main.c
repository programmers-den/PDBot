#include <orca/discord.h>
#include "src/libs/bot_include.h"

int main() {
    discord_global_init();
    struct discord *client = discord_config_init("bot.config");

    discord_set_on_ready(client, &on_ready);
    discord_set_on_message_create(client, &on_message_create);
    discord_set_on_message_delete(client, &on_message_delete);
    discord_set_on_message_update(client, &on_message_update);
    discord_set_on_message_reaction_add(client, &on_message_reaction_add);
    discord_set_on_message_reaction_remove(client, &on_message_reaction_remove);
    discord_set_on_guild_member_add(client, &on_guild_member_add);
    discord_set_on_guild_member_remove(client, &on_guild_member_remove);
    discord_set_on_voice_state_update(client, &on_voice_state_update);

    discord_set_on_command(client, "help", &on_help);
    discord_set_on_command(client, "ping", &on_ping);
    discord_set_on_command(client, "icon", &on_icon);
    discord_set_on_command(client, "stat", &on_stat);

    discord_run(client);

    discord_cleanup(client);
    discord_global_cleanup();

    return 0;
}
