#include <stdio.h>
#include <json-c/json.h>
#include "../libs/bot_include.h"

void check_for_json() {
    FILE *fp = fopen(HELP_JSON, "r");
    printf("Loading commands:\n\n");

    if (!fp) {
        printf("❌ Failed to open %s\n❗ Creating %s\n", HELP_JSON, HELP_JSON);
        fp = fopen(HELP_JSON, "w");
        fclose(fp);
        struct json_object *parsed_json = json_tokener_parse(HELP_JSON_DEFAULT);
        printf("✔️  Created %s\n\n", HELP_JSON);
        json_object_to_file_ext(HELP_JSON, parsed_json, 0);
    }
    else fclose(fp);

    fp = fopen(HELP_JSON, "r");

    if (fp) {
        char buffer[1024], files[1024];
        struct json_object *parsed_json = json_tokener_parse(buffer);
        struct json_object *commands;

        get_files(files, "src/commands/");
        fread(buffer, 1024, 1, fp);
        fclose(fp);

        json_object_object_get_ex(parsed_json, "description", &commands);
        json_object_set_string(commands, files);
        json_object_to_file_ext(HELP_JSON, parsed_json, 0);
    }

    return;
}
