#include <orca/discord.h>
#include "../libs/bot_include.h"

void on_voice_state_update(struct discord *client, const struct discord_user *bot, const struct discord_voice_state *vs) {
    if (vs->deaf || vs->mute || vs->self_deaf || vs->self_mute || vs->self_stream || vs->self_video || vs->supress) return;
    else if (vs->channel_id) {
        switch (vs->channel_id) {
            case VC_CHAT_ONE: discord_add_guild_member_role(client, vs->guild_id, vs->user_id, R_VC_CHAT_ONE); break;
            case VC_CHAT_TWO: discord_add_guild_member_role(client, vs->guild_id, vs->user_id, R_VC_CHAT_TWO); break;
            case VC_MUSIC: discord_add_guild_member_role(client, vs->guild_id, vs->user_id, R_VC_MUSIC); break;
            default: break;
        }
    }

    for (size_t i=0; vs->member->roles[i]; i++) {
        switch (vs->member->roles[i]->value) {
            case R_VC_CHAT_ONE: discord_remove_guild_member_role(client, vs->guild_id, vs->user_id, R_VC_CHAT_ONE); break;
            case R_VC_CHAT_TWO: discord_remove_guild_member_role(client, vs->guild_id, vs->user_id, R_VC_CHAT_TWO); break;
            case R_VC_MUSIC: discord_remove_guild_member_role(client, vs->guild_id, vs->user_id, R_VC_MUSIC); break;
            default: break;
        }
    }

    return;
}
