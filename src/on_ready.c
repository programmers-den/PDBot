#include "orca/discord.h"

void on_ready(struct discord *client, const struct discord_user *user) {
    printf("%s%s connected successfully\n", user->username, user->discriminator);
}
