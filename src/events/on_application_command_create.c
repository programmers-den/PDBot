#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_application_command_create(struct discord *client) {
    struct discord_application_command application_command;
    struct discord_ret_application_command ret_application_command = {.sync = &application_command};
    struct discord_edit_application_command_permissions command_permissions = {
        .size = 1,
        .array = &(struct discord_application_command_permission) {
            .permissions = &(discord_application_command_permissions) { 
                .size = 1,
                .array = &(struct discord_application_command_permission) {
                    .id = R_OWNER,
                    .type = DISCORD_APPLICATION_PERMISSION_ROLE,
                    .permission = true,
                }
            }
        }
    };
    struct discord_create_guild_application_command params = {
        .type = DISCORD_APPLICATION_CHAT_INPUT,
        .name = "add_role_all_user",
        .description = "Adds a role to all users",
        .default_permission = false,
        .options = (struct discord_application_command_option *[]) {
            &(struct discord_application_command_option) {
                .type = DISCORD_APPLICATION_OPTION_ROLE,
                .name = "id",
                .description = "Role ID",
                .required = true
            },
            NULL
        }
    };

    discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, &ret_application_command);
    discord_edit_application_command_permissions(client, ID_APPLICATION, ID_GUILD, application_command.id, &command_permissions, NULL); 
    discord_application_command_cleanup(&application_command);

    params.name = "rm_role_all_user";
    params.description = "Removes a role from all users";

    discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, &ret_application_command);
    discord_edit_application_command_permissions(client, ID_APPLICATION, ID_GUILD, application_command.id, &command_permissions, NULL); 
    discord_application_command_cleanup(&application_command);

    params.name = "get_messages";
    params.description = "Recieve your messages that are stored";
    params.default_permission = true;
    params.options = NULL;

    discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, NULL);

    params.name = "help";
    params.description = "Help menu";

    discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, NULL);

    params.name = "icon";
    params.description = "Display user icon";

    discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, NULL);

    params.name = "ping";
    params.description = "Pings the bot";

    discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, NULL);

    params.name = "stats";
    params.description = "Checks the user status";

    discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, NULL);

    command_permissions.permissions[0]->id = ID_OWNER;
    command_permissions.permissions[0]->type = DISCORD_APPLICATION_PERMISSION_USER;
    params.name = "update";
    params.description = "Updates the bot";
    params.default_permission = false;

    discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, &ret_application_command);
    discord_edit_application_command_permissions(client, ID_APPLICATION, ID_GUILD, application_command.id, &command_permissions, NULL); 
    discord_application_command_cleanup(&application_command);

    return;
}
