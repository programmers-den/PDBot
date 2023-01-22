#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_application_command_create(struct discord *client) {
    struct discord_application_command application_command;
    struct discord_ret_application_command ret_application_command = {.sync = &application_command};
    struct discord_create_global_application_command params = {
        .type = DISCORD_APPLICATION_CHAT_INPUT,
        .name = "get_messages",
        .description = "Recieve your messages that are stored",
        .default_permission = true,
        .options = NULL
    };

    discord_create_global_application_command(client, ID_APPLICATION, &params, NULL);

    params.name = "icon";
    params.description = "Display user icon";

    discord_create_global_application_command(client, ID_APPLICATION, &params, NULL);

    params.name = "ping";
    params.description = "Pings the bot";

    discord_create_global_application_command(client, ID_APPLICATION, &params, NULL);

    params.name = "stats";
    params.description = "Checks the user status";

    discord_create_global_application_command(client, ID_APPLICATION, &params, NULL);

    return;
}
