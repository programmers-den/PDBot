#include <stddef.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>

struct discord_embed *load_embed_from_json(char *filename) {
    size_t len;
    char *json_payload = orka_load_whole_file(filename, &len);

    struct discord_embed *new_embed = discord_embed_alloc();
    discord_embed_from_json(json_payload, len, new_embed);

    new_embed->timestamp = orka_timestamp_ms();

    free(json_payload);

    return new_embed;
}
