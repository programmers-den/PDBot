target = main
compiler = gcc
src = $(wildcard src/*/*.c)
obj = $(src:.c=.o)
flags = -O2 -ldiscord -lcurl -lcrypto -lssl -lm -lpthread -pthread -lsqlite3 `pkg-config --cflags --libs json-c`
all: $(target)

$(target): main.c $(obj)
	$(compiler) $^ $(flags) -o $@

%.o: %.c
	$(compiler) -c $(flags) $^ -o $@

clean:
	rm -rfv $(obj) $(target)
