#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_interaction_create(struct discord *client, const struct discord_interaction *interaction) {
	if (interaction->type != DISCORD_INTERACTION_APPLICATION_COMMAND) return;

	if (!strcmp(interaction->data->name, "add_role_all_user")) add_role_all_user(client, interaction);
	else if (!strcmp(interaction->data->name, "rm_role_all_user")) rm_role_all_user(client, interaction);
	else if (!strcmp(interaction->data->name, "get_messages")) get_messages(client, interaction);
	else if (!strcmp(interaction->data->name, "help")) help(client, interaction);
	else if (!strcmp(interaction->data->name, "icon")) icon(client, interaction);
	else if (!strcmp(interaction->data->name, "ping")) ping(client, interaction);
	else if (!strcmp(interaction->data->name, "stats")) stats(client, interaction);
	else if (!strcmp(interaction->data->name, "update")) update(client, interaction);
}
