#include <regex.h>
#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

size_t get_args_len(const struct discord_message *message, char *split) {
    size_t args_count = 0;
    char tmpMsg[2001];
    strcpy(tmpMsg, message->content);
    char *tok = strtok(tmpMsg, split);

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
    char tmpMsg[2001];
    strcpy(tmpMsg, message->content);
    char *tok = strtok(tmpMsg, split);
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

size_t check_if_args_ids(const struct discord_message *message, char *split) {
    // checks if the message contents are all valid ids
    // returns 0 if valid, returns index if not valid
    char tmpMsg[2001];
    strcpy(tmpMsg, message->content);
    char *tok = strtok(tmpMsg, split);

    if (!tok) return 1;

    regex_t regex;
    regcomp(&regex, REGEX_ID, REG_EXTENDED|REG_NOSUB);

    for (size_t i=0; tok; i++) {
        if (regexec(&regex, tok, 0, NULL, 0)) {
            regfree(&regex);
            return i+1;
        }

        tok = strtok(NULL, split);
    }

    regfree(&regex);

    return 0;
}

u64_snowflake_t get_args_ids_as_int_at(const struct discord_message *message, size_t argc, char *split) {
    // returns 1 if successful, 0 if not
    char tmpMsg[2001];
    strcpy(tmpMsg, message->content);
    char *tok = strtok(tmpMsg, split);

    if (!tok) return false;

    regex_t regex;
    regcomp(&regex, REGEX_ID, REG_EXTENDED|REG_NOSUB);

    for (size_t i=0; tok; i++) {
        if ((i == argc-1) && !regexec(&regex, tok, 0, NULL, 0)) {
            return strtoul(tok, NULL, 10);
        }

        tok = strtok(NULL, split);
    }

    regfree(&regex);

    return 0;
}

uint8_t get_args_ids_as_str_at(char *buf, const struct discord_message *message, size_t argc, char *split) {
    // buf[ID_STR_LEN]
    // returns 1 if successful, 0 if not
    char tmpMsg[2001];
    strcpy(tmpMsg, message->content);
    char *tok = strtok(tmpMsg, split);

    if (!tok) return false;
    if (buf) memset(buf, 0, MAX_GUILD_ROLES);

    regex_t regex;
    regcomp(&regex, REGEX_ID, REG_EXTENDED|REG_NOSUB);

    for (size_t i=0; tok; i++) {
        if ((i == argc) && !regexec(&regex, tok, 0, NULL, 0)) {
            if (buf) strcpy(buf, tok);
        }
        else {
            regfree(&regex);
            return false;
        }

        tok = strtok(NULL, split);
    }

    regfree(&regex);

    return 0;
}
