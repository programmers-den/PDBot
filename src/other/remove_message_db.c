#include <sqlite3.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

void remove_message_db(u64_snowflake_t message_id) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(BOT_DB, &db);
    if (rc) printf("\nFailed to open %s!\n\n", BOT_DB);
    else {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("DELETE FROM %s WHERE message_id = %lu", MESSAGE_TABLE, message_id);
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
