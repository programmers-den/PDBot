#include <orca/discord.h>
#include "../libs/bot_include.h"

void on_ready(struct discord *client, const struct discord_user *bot) {
    char username_and_descrim[USER_AND_DESCRIM_LEN];
    username_and_discriminator_to_str(username_and_descrim, bot);
    printf("%s connected successfully\n\n", username_and_descrim);

    check_for_json();

    printf("\nLoading events:\n\n");
    get_files(NULL, "src/events/");

    printf("\nLoading etc:\n\n");
    get_files(NULL, "src/other/");

    check_for_db();

    printf("\n✔️  Ready!\n\n");

    return;
}
