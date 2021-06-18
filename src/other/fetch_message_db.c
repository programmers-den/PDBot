#include <string.h>
#include <sqlite3.h>
#include <orca/discord.h>
#include <orca/orka-utils.h>
#include "../libs/config.h"

struct discord_message *fetch_message_db(struct discord *client, u64_snowflake_t guild_id, u64_snowflake_t message_id) {
    struct discord_message *message = discord_message_alloc();
    message->content = malloc(1);
    memset(message->content, 0, 1);
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_open(BOT_DB, &db);
    if (rc) printf("\nFailed to open %s!\n\n", BOT_DB);
    else {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("SELECT timestamp, author_id, message_id, content FROM %s WHERE message_id = %lu;", MESSAGE_TABLE, message_id);
        // puts(query);
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

        while ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
            u64_snowflake_t db_message_id = 0;
            // printf("Timestamp: %lu, Author_id: %lu, Message_id: %lu, Content: %s\n", sqlite3_column_int64(stmt, 0), sqlite3_column_int64(stmt, 1), sqlite3_column_int64(stmt, 2), sqlite3_column_text(stmt, 3));
            if ((db_message_id = sqlite3_column_int64(stmt, 2)) == message_id) {
                printf("%lu, %lu\n", db_message_id, message_id);
                struct discord_user *member = discord_user_alloc();
                message->timestamp = sqlite3_column_int64(stmt, 0);
                message->author->id = sqlite3_column_int64(stmt, 1);
                discord_get_user(client, message->author->id, member);
                message->author = member;
                message->id = db_message_id;
                message->content = malloc(4001);
                strcpy(message->content, sqlite3_column_text(stmt, 3));
            }
        }
        // if (rc) {
        //     printf("\nSQL Error: %s\n\n", errMsg);
        //     sqlite3_free(errMsg);
        // }

        sqlite3_free(query);
        sqlite3_close(db);
    }

    return message;
}
