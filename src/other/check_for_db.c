#include <stdio.h>
#include <sqlite3.h>
#include "../libs/config.h"

void check_for_db() {
    FILE *fp = fopen(BOT_DB, "r");
    if (fp) printf("✔️  Found db: %s\n", BOT_DB);
    else {
        printf("❌ Failed to open db: %s\n❗ Creating %s\n", BOT_DB, BOT_DB);
        sqlite3 *db = NULL;
        int rc = sqlite3_open(BOT_DB, &db);

        if (rc) printf("❌ Failed to create %s\n\n", BOT_DB);
        else {
            printf("✔️  Created db: %s\n❗ Creating tables\n", BOT_DB);
            char *query = NULL, *errMsg = NULL;
            query = sqlite3_mprintf("CREATE TABLE %s (timestamp, author_id, message_id, content)", MESSAGE_TABLE);
            rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
            sqlite3_free(query);

            if (rc != SQLITE_OK) {
                printf("\nSQL Error: %s\n\n", errMsg);
                sqlite3_free(errMsg);
            }
            else printf("✔️  Created table: %s\n", MESSAGE_TABLE);

            sqlite3_close(db);
        }
    }

    return;
}