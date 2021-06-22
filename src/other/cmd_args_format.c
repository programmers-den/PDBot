#include <regex.h>
#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

size_t get_args_len(const struct discord_message *message, char *split) {
    size_t args_count = 0;
    char *tok = strtok(message->content, split);

    if (tok) {
        while (tok) {
            args_count++;
            tok = strtok(NULL, split);
        }
    }

    return args_count;
}

uint8_t get_arg_at(char *buf, const struct discord_message *message, size_t argc, char *split) {
    // buf[MAX_CMD_ARGS_LEN]
    // returns 1 if successful, 0 if not
    char *tok = strtok(message->content, split);
    memset(buf, 0, MAX_CMD_ARGS_LEN);

    if (!tok) return false;

    for (size_t i=0; tok; i++) {
        if (i == argc-1) {
            strcpy(buf, tok);

            break;
        }
            
        tok = strtok(NULL, split);
    }

    return true;
}

size_t get_args_ids_as_int(u64_snowflake_t *buf, const struct discord_message *message, char *split) {
    // buf[MAX_GUILD_ROLES]
    // returns 0 if successful, returns arg index that failed to parse if not successful
    regex_t regex;
    char *tok = strtok(message->content, split);

    if (buf) memset(buf, 0, MAX_GUILD_ROLES);
    if (!tok) return 1;

    regcomp(&regex, REGEX_ID, REG_EXTENDED|REG_NOSUB);

    for (size_t i=0; tok; i++) {
        if (!regexec(&regex, tok, 0, NULL, 0)) {
            if (buf) buf[i] = strtoul(tok, NULL, 10);
        }
        else {
            regfree(&regex);
            return i+1;
        }

        tok = strtok(NULL, split);
    }

    regfree(&regex);

    return 0;
}

uint8_t get_args_ids_as_str_at(char *buf, const struct discord_message *message, size_t argc, char *split) {
    // buf[ID_STR_LEN]
    // returns 1 if successful, 0 if not
    regex_t regex;
    char *tok = strtok(message->content, split);

    if (!tok) return false;

    for (size_t i=0; tok; i++) {
        if ((i == argc) && !regcomp(&regex, REGEX_ID, 0)) {
            memset(buf, 0, MAX_GUILD_ROLES_STR);
            strcpy(buf, tok);

            tok = strtok(NULL, split);

        }
    }

    return true;
}
