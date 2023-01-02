#include <signal.h>
#include <concord/discord.h>
#include <concord/log.h>
#include "../libs/bot_include.h"

extern struct discord *client;

void on_sigint(int signum) {
  (void)signum;
  log_info("SIGINT received, shutting down ...");
  close_db();
  discord_shutdown(client);
}

