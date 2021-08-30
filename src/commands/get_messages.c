#include <regex.h>
#include <orca/discord.h>
#include "../libs/bot_include.h"

static int callback(void *handle, int argc, char **argv, char **azColName);

void get_messages(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {
    if (msg->author->bot) return;

    sqlite3 *db = NULL;
    char *errMsg = NULL, *query = NULL, *filename = NULL;
    int rc = sqlite3_open(BOT_DB, &db);
    struct discord_channel dm_channel;
    struct discord_embed embed;
    discord_channel_init(&dm_channel);
    discord_embed_init(&embed);
    struct discord_create_message_params params = {.embed = &embed};

    filename = sqlite3_mprintf("%s.csv", msg->author->username);
    FILE *fp = fopen(filename, "w");

    if (rc) failed_message(client, &embed, &params, (char*)sqlite3_errmsg(db), msg->channel_id);
    else {
        fprintf(fp, "\"timestamp\", \"author_id\", \"message_id\", \"content\", \n");
        query = sqlite3_mprintf("SELECT timestamp, author_id, message_id, content FROM %s WHERE author_id = %lu;", MESSAGE_TABLE, msg->author->id);
        rc = sqlite3_exec(db, query, &callback, fp, &errMsg);
        fclose(fp);

        if (rc != SQLITE_OK) {
            failed_message(client, &embed, &params, errMsg, msg->channel_id);
            sqlite3_free(errMsg);
        }
        else {
            size_t filename_len = strlen(filename);
            embed.color = COLOR_MINT;
            snprintf(embed.title, sizeof(embed.title), "Sent!");
            snprintf(embed.description, sizeof(embed.description), "Please check your dms from the bot");

            discord_create_dm(client, msg->author->id, &dm_channel);
            discord_create_message(client, msg->channel_id, &params, NULL);

            params.embed = NULL;
            params.file.name = filename;
            discord_create_message(client, dm_channel.id, &params, NULL);

            remove(params.file.name);
        }
    }

    sqlite3_close(db);
    discord_channel_cleanup(&dm_channel);
    discord_embed_cleanup(&embed);

    return;
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
