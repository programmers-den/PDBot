#include <concord/discord.h>

uint8_t guild_member_has_role(struct discord_guild_member *guild_member, u64snowflake role_id) {
    for (int i=0; i < guild_member->roles->size; i++) {
        if (guild_member->roles->array[i] == role_id) return true;
    }

    return false;
}
