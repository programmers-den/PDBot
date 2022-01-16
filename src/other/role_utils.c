#include <concord/discord.h>

uint8_t guild_member_has_role(struct discord_guild_member *guild_member, u64_snowflake_t role_id) {
    for (size_t i=0; guild_member->roles[i]; i++) {
        if (guild_member->roles[i]->value == role_id) return true;
    }

    return false;
}
