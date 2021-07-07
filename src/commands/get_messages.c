#include <orca/discord.h>
#include "../libs/bot_include.h"

void get_messages(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {
    if (msg->author->bot) return;

    sqlite3 *db = NULL;
    char *errMsg = NULL, *query = NULL;
    int rc = sqlite3_open(BOT_DB, &db);
    struct discord_channel *dm_channel = discord_channel_alloc();
    struct discord_embed *embed = discord_embed_alloc();
    struct discord_create_message_params params = {.embed = embed};

    if (rc) failed_message(client, embed, &params, (char*)sqlite3_errmsg(db), msg->channel_id);

    rc = sqlite3_exec(db, ".headers on", NULL, NULL, &errMsg);

    if (rc != SQLITE_OK) {
        failed_message(client, embed, &params, errMsg, msg->channel_id);
        sqlite3_free(errMsg);
    }
    else {
        rc = sqlite3_exec(db, ".mode csv", NULL, NULL, &errMsg);

        if (rc != SQLITE_OK) {
            failed_message(client, embed, &params, errMsg, msg->channel_id);
            sqlite3_free(errMsg);
        }
        else {
            sqlite3_mprintf(".output %s.csv", msg->author->username);
            rc = sqlite3_exec(db, ".output ", NULL, NULL, &errMsg);

            if (rc != SQLITE_OK) {
                failed_message(client, embed, &params, errMsg, msg->channel_id);
                sqlite3_free(errMsg);
            }
            else {
                query = sqlite3_mprintf("SELECT timestamp, author_id, message_id, content FROM %s WHERE message_id = %lu;", MESSAGE_TABLE, msg->id);
                rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);

                if (rc != SQLITE_OK) {
                    failed_message(client, embed, &params, errMsg, msg->channel_id);
                    sqlite3_free(errMsg);
                }
                else {
                    query = sqlite3_mprintf(".output %s.csv", msg->author->username);

                    size_t filename_len = strlen(query);
                    embed->color = COLOR_MINT;
                    snprintf(embed->title, 257, "Sent!");
                    snprintf(embed->description, 2049, "Please check your dms from the bot: %s", errMsg);
                    params.file.name = sqlite3_mprintf("%s.csv", msg->author->username);

                    discord_create_dm(client, msg->author->id, dm_channel);
                    discord_create_message(client, dm_channel->id, &params, NULL);

                    remove(params.file.name);
                }
            }
        }
    }

    sqlite3_close(db);
    discord_channel_free(dm_channel);
    discord_embed_free(embed);

    return;
}
