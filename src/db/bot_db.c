#include <sqlite3.h>
#include <concord/discord.h>
#include <concord/cog-utils.h>
#include "../libs/bot_include.h"

extern sqlite3 *db;
extern FILE *dbFile;
extern int dbIsOpen;

void ready_db() {
    if (!dbIsOpen) {
        dbFile = fopen(BOT_DB, "r");
        if (dbFile) {
            printf("✔️  Found DB: %s\n", BOT_DB);
            fclose(dbFile);
            int rc = sqlite3_open(BOT_DB, &db);
            dbIsOpen = 1;
        }
        else {
            printf("❌ Failed to open DB: %s\n❗ Creating %s\n", BOT_DB, BOT_DB);
            int rc = sqlite3_open(BOT_DB, &db);
            if (rc) printf("❌ Failed to create %s (PROGRAM CHECK!)\n\n", BOT_DB);
            else {
                printf("✔️  Created a new SQLite3 DB: %s\n❗ Creating tables...\n", BOT_DB);
                char *query = NULL, *errMsg = NULL;
                query = sqlite3_mprintf("CREATE TABLE %s (timestamp, author_id, message_id, content)", MESSAGE_TABLE);
                rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
                sqlite3_free(query);

                if (rc != SQLITE_OK) {
                    printf("\nSQL Error: %s\n\n", errMsg);
                    sqlite3_free(errMsg);
                }
                else printf("✔️  Created table: %s\n", MESSAGE_TABLE);
                dbIsOpen = 1;
            }
        }
    }
    else {
        printf("✔️  DB is already open: %s\n", BOT_DB);
    }
    return;
}

void close_db() {
    if (dbIsOpen) {
        printf("✔️  Closed DB: %s\n\n", BOT_DB);
        sqlite3_close(db);
        dbIsOpen = 0;
    }
    else {
        printf("❌ Tried to close a non-open DB!\n\n");
    }
}


struct discord_message fetch_message_db(struct discord *client, u64_snowflake_t guild_id, u64_snowflake_t message_id) {
    struct discord_message message;
    discord_message_init(&message);
    message.content = malloc(1);
    memset(message.content, 0, 1);
    sqlite3_stmt *stmt = NULL;
    if (dbIsOpen == 0) {
        ready_db();
    }
    if (db != NULL) {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("SELECT timestamp, author_id, message_id, content FROM %s WHERE message_id = %lu;", MESSAGE_TABLE, message_id);
        int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
        while ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
            u64_snowflake_t db_message_id = 0;
            // printf("Timestamp: %lu, Author_id: %lu, Message_id: %lu, Content: %s\n", sqlite3_column_int64(stmt, 0), sqlite3_column_int64(stmt, 1), sqlite3_column_int64(stmt, 2), sqlite3_column_text(stmt, 3));
            if ((db_message_id = sqlite3_column_int64(stmt, 2)) == message_id) {
                free(message.content);
                printf("%lu, %lu\n", db_message_id, message_id);
                struct discord_user *member = malloc(sizeof(*member));
                struct discord_ret_user ret_user = {.sync = member};
                discord_user_init(member);
                message.timestamp = sqlite3_column_int64(stmt, 0);
                member->id = sqlite3_column_int64(stmt, 1);
                discord_get_user(client, member->id, &ret_user);
                message.author = member;
                message.id = db_message_id;
                message.content = malloc(4001);
                strcpy(message.content, (const char*)sqlite3_column_text(stmt, 3));
            }
        }
        // if (rc) {
        //     printf("\nSQL Error: %s\n\n", errMsg);
        //     sqlite3_free(errMsg);
        // }
        sqlite3_free(query);
    }
    return message;
}

void add_message_db(const struct discord_message *message) {
    if (!message->content[0]) return;
    if (!dbIsOpen) {
        ready_db();
    }
    if (db != NULL) {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("INSERT INTO %s(timestamp, author_id, message_id, content) values(%lu, %lu, %lu, '%s');", MESSAGE_TABLE, cog_timestamp_ms(), message->author->id, message->id, message->content);
        int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
        sqlite3_free(query);
        if (rc != SQLITE_OK) {
            printf("\nSQL Error: %s\n\n", errMsg);
            sqlite3_free(errMsg);
        }
    }
    return;
}

void remove_message_db(u64_snowflake_t message_id) {
    if (!dbIsOpen) {
        ready_db();
    }
    if (db != NULL) {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("DELETE FROM %s WHERE message_id = %lu", MESSAGE_TABLE, message_id);
        int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
        sqlite3_free(query);
        if (rc != SQLITE_OK) {
            printf("\nSQL Error: %s\n\n", errMsg);
            sqlite3_free(errMsg);
        }
    }
    return;
}

void update_message_db(const struct discord_message *message) {
    if (!message->content[0]) return;
    if (!dbIsOpen) {
        ready_db();
    }
    if (db != NULL) {
        char *query = NULL, *errMsg = NULL;
        query = sqlite3_mprintf("UPDATE %s SET content = '%s' WHERE message_id = %lu;", MESSAGE_TABLE, message->content, message->id);
        int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
        sqlite3_free(query);
        if (rc != SQLITE_OK) {
            printf("\nSQL Error: %s\n\n", errMsg);
            sqlite3_free(errMsg);
        }
    }
    return;
}
