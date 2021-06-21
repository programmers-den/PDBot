#include <orca/discord.h>
#include "../libs/bot_include.h"

void get_avatar_url(char *buf, const struct discord_user *user) {
    // buf[AVATAR_URL_LEN]
    memset(buf, 0, AVATAR_URL_LEN);
    snprintf(buf, AVATAR_URL_LEN, "https://cdn.discordapp.com/avatars/%lu/%s.png", user->id, user->avatar);

    return;
}
