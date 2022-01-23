#include <regex.h>
#include <concord/discord.h>
#include "../libs/bot_include.h"

static int callback(void *handle, int argc, char **argv, char **azColName);

void get_messages(struct discord *client, const struct discord_interaction *interaction) {
    sqlite3 *db = NULL;
    char *errMsg = NULL, *query = NULL, *filename = NULL;
    int rc = sqlite3_open(BOT_DB, &db);
    struct discord_attachment attachment;
    discord_attachment_init(&attachment);
    struct discord_interaction_response interaction = {
	    .type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE,
	    .data = &(struct discord_interaction_callback_data) {.attachments = (struct discord_attachment *[]) {&attachment, NULL}}
    };

    filename = sqlite3_mprintf("%s.csv", interaction->member->user->username);
    FILE *fp = fopen(filename, "w");

    if (rc) {
	    interaction.data->content = (char*)sqlite3_errmsg(db);
	    interaction.data->attachments = NULL;

	    discord_create_interaction_response(client, interaction->id, interaction->token, &interaction, NULL);
    }
    else {
        fprintf(fp, "\"timestamp\", \"author_id\", \"message_id\", \"content\", \n");
        query = sqlite3_mprintf("SELECT timestamp, author_id, message_id, content FROM %s WHERE author_id = %lu;", MESSAGE_TABLE, interaction->member->user->id);
        rc = sqlite3_exec(db, query, &callback, fp, &errMsg);
        fclose(fp);

        if (rc != SQLITE_OK) {
            interaction.data->content = errMsg;
            sqlite3_free(errMsg);
        }
        else {
	    interaction.data->content = filename;
	    attachment.filename = filename;

	    discord_create_interaction_response(client, interaction->id, interaction->token, &interaction, NULL);

            remove(attachment.filename);
        }
    }

    sqlite3_close(db);

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
