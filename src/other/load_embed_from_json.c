#include <orca/discord.h>
#include <orca/cee-utils.h>

struct discord_embed *load_embed_from_json(char *filename) {
    size_t len = 0;
    char *json_payload = cee_load_whole_file(filename, &len);

    struct discord_embed *new_embed = discord_embed_alloc();
    discord_embed_from_json(json_payload, len, new_embed);

    new_embed->timestamp = cee_timestamp_ms();

    free(json_payload);

    return new_embed;
}
