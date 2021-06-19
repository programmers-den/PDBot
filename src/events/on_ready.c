#include <stdio.h>
#include <orca/discord.h>
#include "../libs/get_files.h"
#include "../libs/check_for_json.h"
#include "../libs/check_for_db.h"

void on_ready(struct discord *client, const struct discord_user *user) {
    printf("%s%s connected successfully\n\n", user->username, user->discriminator);

    check_for_json();

    printf("\nLoading events:\n\n");
    get_files(NULL, "src/events/");

    printf("\nLoading etc:\n\n");
    get_files(NULL, "src/other/");

    check_for_db();

    printf("\n✔️  Ready!\n\n");

    return;
}
