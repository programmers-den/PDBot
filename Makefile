target = main
compiler = gcc
src = $(wildcard src/*/*.c)
obj = $(src:.c=.o)
flags = -g -O3 -lcurl -lcrypto -lssl -lm -ldiscord -lpthread -pthread `pkg-config --cflags --libs json-c`
all: $(target)

$(target): main.c $(obj)
	$(compiler) $^ $(flags) -o $@

%.o: %.c
	$(compiler) -c $(flags) $^ -o $@

clean:
	rm -rfv $(obj) $(target)
