#include <string.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>

char *get_icon_url(const struct discord_user *user) {
    size_t len = strlen(user->avatar)+41+18; // url + id
    char *url = malloc(len);
    snprintf(url, len, "https://cdn.discordapp.com/avatars/%lu/%s.png", user->id, user->avatar);

    return url;
}
