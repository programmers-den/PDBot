#include <stdlib.h>
#include <stdio.h>
#include <orca/discord.h>
#include "src/libs/on_ready.h"
#include "src/libs/help.h"
#include "src/libs/ping.h"

int main() {
    discord_global_init();
    struct discord *client = discord_config_init("bot.config");

    discord_set_on_ready(client, &on_ready);
    discord_set_on_command(client, "help", &help);
    discord_set_on_command(client, "ping", &ping);

    discord_run(client);

    discord_cleanup(client);
    discord_global_cleanup();

    return 0;
}
