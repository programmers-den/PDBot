#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_application_command_create(struct discord *client) {
	struct discord_create_guild_application_command_params params = {
		.type = DISCORD_APPLICATION_COMMAND_CHAT_INPUT,
		.name = "add_role_all_user",
		.description = "Adds a role to all users",
		.default_permission = true,
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

	discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, NULL);

	params.name = "rm_role_all_user";
	params.description = "Removes a role from all users";

	discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, NULL);

	params.name = "get_messages";
	params.description = "Recieve your messages that are stored";
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

	params.name = "update";
	params.description = "Updates the bot";

	discord_create_guild_application_command(client, ID_APPLICATION, ID_GUILD, &params, NULL);
}
