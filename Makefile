TARGET = main
CC = gcc
OBJS = \
src/commands/add_role_all_user.o src/events/on_channel_delete.o \
src/commands/get_messages.o src/events/on_channel_update.o src/db/bot_db.o \
src/commands/help.o src/events/on_guild_member_add.o src/other/check_for_json.o \
src/commands/icon.o src/events/on_guild_member_remove.o src/other/cmd_args_format.o \
src/commands/ping.o src/events/on_interaction_create.o src/other/format_message.o \
src/commands/rm_role_all_user.o src/events/on_message_create.o src/other/get_avatar_url.o \
src/commands/stats.o src/events/on_message_delete.o src/other/get_files.o \
src/commands/update.o src/events/on_message_reaction_add.o src/other/get_message_url.o \
src/events/on_message_reaction_remove.o src/other/load_embed_from_json.o \
src/events/on_application_command_create.o src/events/on_message_update.o src/other/role_utils.o \
src/events/on_channel_create.o src/events/on_ready.o src/signals/on_sigint.o
RUNOBJS = main.o
CFLAGS = -I/usr/local/include -g
LINKLIBS = -g -ldiscord -lcurl -lm -lpthread -pthread -lsqlite3 -ljson-c -L/usr/local/lib

all: $(TARGET)

$(TARGET): $(RUNOBJS) $(OBJS)
	$(CC) $(OBJS) $(RUNOBJS) $(LINKLIBS) -o main

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rfv $(OBJS) $(RUNOBJS) $(TARGET)
