#include <stdlib.h>
#include <concord/discord.h>
#include <concord/cog-utils.h>

struct discord_embed *load_embed_from_json(char *filename) {
    size_t len = 0;
    char *json_payload = cog_load_whole_file(filename, &len);

    struct discord_embed *new_embed = malloc(sizeof(*new_embed));
    discord_embed_from_json(json_payload, len, new_embed);

    new_embed->timestamp = cog_timestamp_ms();

    free(json_payload);
    free(new_embed);

    return new_embed;
}
