#include <stdio.h>
#include <orca/discord.h>
#include <json-c/json.h>
#include "../libs/get_commands.h"

void on_ready(struct discord *client, const struct discord_user *user) {
    printf("%s%s connected successfully\n\n", user->username, user->discriminator);
    FILE *fp = fopen("src/json/help.json", "r");

    if (fp) {
        char buffer[1024];
        fread(buffer, 1024, 1, fp);
        fclose(fp);
        struct json_object *parsed_json = json_tokener_parse(buffer);
        struct json_object *commands;
        json_object_object_get_ex(parsed_json, "description", &commands);
        json_object_set_string(commands, get_commands("src/commands/"));
        json_object_to_file_ext("src/json/help.json", parsed_json, 0);
    }
    else printf("Failed to load json file\n");
    printf("\nReady!\n");

    return;
}
