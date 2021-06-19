#include <orca/discord.h>
#include <sqlite3.h>
#include "../libs/config.h"

void update_message_db(const struct discord_message *message) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(BOT_DB, &db);

    if (rc) printf("\nFailed to open %s!\n\n", BOT_DB);
    else {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("UPDATE %s SET content = '%s' WHERE message_id = %lu;", MESSAGE_TABLE, message->content, message->id);
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
