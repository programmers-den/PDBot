#include <stdlib.h>
#include <stdio.h>
#include <orca/discord.h>
#include "src/libs/on_ready.h"
#include "src/libs/on_message_create.h"
#include "src/libs/on_message_delete.h"
#include "src/libs/on_message_reaction_add.h"
#include "src/libs/on_message_reaction_remove.h"
#include "src/libs/on_voice_state_update.h"
#include "src/libs/on_guild_member_add.h"
#include "src/libs/on_guild_member_remove.h"
#include "src/libs/help.h"
#include "src/libs/ping.h"
#include "src/libs/icon.h"
#include "src/libs/stat.h"

int main() {
    discord_global_init();
    struct discord *client = discord_config_init("bot.config");

    discord_set_on_ready(client, &on_ready);
    discord_set_on_voice_state_update(client, &on_voice_state_update);
    discord_set_on_message_create(client, &on_message_create);
    discord_set_on_message_delete(client, &on_message_delete);
    discord_set_on_message_reaction_add(client, &on_message_reaction_add);
    discord_set_on_message_reaction_remove(client, &on_message_reaction_remove);
    discord_set_on_guild_member_add(client, &on_guild_member_add);
    discord_set_on_guild_member_remove(client, &on_guild_member_remove);
    discord_set_on_command(client, "help", &help);
    discord_set_on_command(client, "ping", &ping);
    discord_set_on_command(client, "icon", &icon);
    discord_set_on_command(client, "stat", &stat);

    discord_run(client);

    discord_cleanup(client);
    discord_global_cleanup();

    return 0;
}
