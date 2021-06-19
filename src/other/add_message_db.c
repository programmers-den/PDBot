#include <sqlite3.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

void add_message_db(const struct discord_message *message) {
    if (!message->content[0]) return;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(BOT_DB, &db);

    if (rc) printf("\nFailed to open %s!\n\n", BOT_DB);
    else {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("INSERT INTO %s(timestamp, author_id, message_id, content) values(%lu, %lu, %lu, '%s');", MESSAGE_TABLE, orka_timestamp_ms(), message->author->id, message->id, message->content);
        rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
        sqlite3_free(query);
        if (rc != SQLITE_OK) {
            printf("\nSQL Error: %s\n\n", errMsg);
            sqlite3_free(errMsg);
        }

        sqlite3_close(db);
    }

    return;
}
