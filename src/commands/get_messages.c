#include <regex.h>
#include <concord/discord.h>
#include "../libs/bot_include.h"

static void file_cleanup(struct discord *client, struct discord_timer *timer);
static int callback(void *handle, int argc, char **argv, char **azColName);

void get_messages(struct discord *client, const struct discord_interaction *interaction) {
    sqlite3 *db = NULL;
    char *errMsg = NULL, *query = NULL, *filename = NULL;
    int rc = sqlite3_open(BOT_DB, &db);
    struct discord_attachment attachment = { 0 };
    struct discord_interaction_response interaction_response = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data) {
            .flags = DISCORD_MESSAGE_EPHEMERAL,
            .attachments = &(struct discord_attachments) {
                .size = 1,
                .array = &attachment
            }
        }
    };

    filename = sqlite3_mprintf("%s.csv", interaction->member->user->username);
    FILE *fp = fopen(filename, "w");

    if (rc) {
        if (fp) fclose(fp);

        interaction_response.data->content = (char*)sqlite3_errmsg(db);
        interaction_response.data->attachments = NULL;

        discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_response, NULL);
    }
    else if (fp) {
        fprintf(fp, "\"timestamp\", \"author_id\", \"message_id\", \"content\", \n");
        query = sqlite3_mprintf("SELECT timestamp, author_id, message_id, content FROM %s WHERE author_id = %lu;", MESSAGE_TABLE, interaction->member->user->id);
        rc = sqlite3_exec(db, query, &callback, fp, &errMsg);
        fclose(fp);

        if (rc != SQLITE_OK) {
            interaction_response.data->content = errMsg;
            sqlite3_free(errMsg);
        }
        else {
            struct discord_ret_interaction_response ret_interaction_response = {
                .data = strdup(filename),
                .cleanup = &ret_cleanup
            };

            interaction_response.data->content = attachment.filename = ret_interaction_response.data;

            discord_create_interaction_response(client, interaction->id, interaction->token, &interaction_response, &ret_interaction_response);
        }
    }

    sqlite3_close(db);

    return;
}

void file_cleanup(struct discord *client, void *data) {
    remove(data);
    free(data);
}

static int callback(void *handle, int argc, char **argv, char **azColName) {
    FILE *fp = handle;
    regex_t regex;
    regcomp(&regex, "^\".*\"$", REG_EXTENDED|REG_NOSUB);

    for (int i=0; i<argc; i++) {
        if (regexec(&regex, argv[i], 0, NULL, 0)) fprintf(fp, "\"%s\", ", argv[i]);
        else fprintf(fp, "\"\"%s\"\", ", argv[i]);
    }

    fprintf(fp, "\n");
    regfree(&regex);

    return 0;
}
