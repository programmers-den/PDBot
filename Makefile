target = main
cc = gcc
src = $(wildcard src/*/*.c)
obj = $(src:.c=.o)
flags = -g -ldiscord -lcurl -lcrypto -lssl -lm -lpthread -pthread -lsqlite3 `pkg-config --cflags --libs json-c`
all: $(target)

$(target): main.c $(obj)
	$(cc) $^ $(flags) -o $@

%.o: %.c
	$(cc) -c $(flags) $^ -o $@

clean:
	rm -rfv $(obj) $(target)
