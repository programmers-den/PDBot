#include <string.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

void get_icon_url(char *buf, const struct discord_user *user) {
    // buf[AVATAR_URL_LEN]
    snprintf(buf, AVATAR_URL_LEN, "https://cdn.discordapp.com/avatars/%lu/%s.png", user->id, user->avatar);

    return;
}
