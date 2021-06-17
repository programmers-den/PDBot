#include <string.h>
#include <sqlite3.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

struct discord_message *fetch_message_db(struct discord *client, u64_snowflake_t guild_id, u64_snowflake_t message_id) {
    struct discord_message *message = discord_message_alloc();
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_open(BOT_DB, &db);
    if (rc) printf("\nFailed to open %s!\n\n", BOT_DB);
    else {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("SELECT timestamp, author_id, message_id, content FROM %s WHERE message_id = %lu;", MESSAGE_TABLE, message_id);
        sqlite3_prepare16_v2(db, query, -1, &stmt, NULL);
        while ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
            u64_snowflake_t db_message_id = 0;
            struct discord_user *member = discord_user_alloc();
            if ((db_message_id = sqlite3_column_int64(stmt, 2)) == message_id) {
                message->timestamp = sqlite3_column_int64(stmt, 0);
                discord_get_guild_member(client, guild_id, db_message_id, (struct discord_guild_member*)member);
                message->author = member;
                message->author->id = sqlite3_column_int64(stmt, 1);
                message->id = db_message_id;
                strcpy(message->content, (char*)sqlite3_column_value(stmt, 3));
            }
        }
        sqlite3_free(query);
        if (rc != SQLITE_OK) {
            printf("\nSQL Error: %s\n\n", errMsg);
            sqlite3_free(errMsg);
        }

        sqlite3_close(db);
    }

    return message;
}
