#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_application_command_create(struct discord *client) {
	struct discord_application_command application_command;
	struct discord_edit_application_command_permissions permissions = {
		.id = R_OWNER,
		.type = DISCORD_APPLICATION_COMMAND_PERMISSION_ROLE,
		.permission = true
	};
	struct discord_create_guild_application_command params = {
		.type = DISCORD_APPLICATION_COMMAND_CHAT_INPUT,
		.name = "add_role_all_user",
		.description = "Adds a role to all users",
		.default_permission = false,
		.options = (struct discord_application_command_option *[]) {
			&(struct discord_application_command_option) {
				.type = DISCORD_APPLICATION_COMMAND_OPTION_ROLE,
				.name = "id",
				.description = "Role ID",
				.required = true
			},
			NULL
		}
	};

	discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, &application_command);
	discord_edit_application_command_permissions(client, ID_APPLICATION, ID_GUILD, application_command.id, &permissions, NULL); 
	discord_application_command_cleanup(&application_command);

	params.name = "rm_role_all_user";
	params.description = "Removes a role from all users";

	discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, &application_command);
	discord_edit_application_command_permissions(client, ID_APPLICATION, ID_GUILD, application_command.id, &permissions, NULL); 
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

	permissions.id = ID_OWNER;
	permissions.type = DISCORD_APPLICATION_COMMAND_PERMISSION_USER;
	params.name = "update";
	params.description = "Updates the bot";
	params.default_permission = false;

	discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, &application_command);
	discord_edit_application_command_permissions(client, ID_APPLICATION, ID_GUILD, application_command.id, &permissions, NULL); 
	discord_application_command_cleanup(&application_command);
}
