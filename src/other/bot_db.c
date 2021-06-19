#include <sqlite3.h>
#include <orca/discord.h>
#include <orca/cee-utils.h>
#include "../libs/bot_include.h"

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

void add_message_db(const struct discord_message *message) {
    if (!message->content[0]) return;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(BOT_DB, &db);

    if (rc) printf("\nFailed to open %s!\n\n", BOT_DB);
    else {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("INSERT INTO %s(timestamp, author_id, message_id, content) values(%lu, %lu, %lu, '%s');", MESSAGE_TABLE, cee_timestamp_ms(), message->author->id, message->id, message->content);
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

void update_message_db(const struct discord_message *message) {
    if (!message->content[0]) return;

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
