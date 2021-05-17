#include <string.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

char *get_icon_url(const struct discord_user *user) {
    size_t len = strlen(user->avatar)+AVATAR_URL_LENGTH;
    char *url = malloc(len);
    snprintf(url, len, "https://cdn.discordapp.com/avatars/%lu/%s.png", user->id, user->avatar);

    return url;
}
