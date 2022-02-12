#include <signal.h>
#include <concord/discord.h>
#include "../libs/bot_include.h"

void on_sigint(int signum) {
  (void)signum;
  log_info("SIGINT received, shutting down ...");
  close_db();
  discord_shutdown(client);
}

