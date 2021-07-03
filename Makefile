TARGET = main
CC = gcc
SRC = $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)
FLAGS = -O3 -ldiscord -lcurl -lcrypto -lssl -lm -lpthread -pthread -lsqlite3 `pkg-config --libs --cflags json-c`

all: $(TARGET)

$(TARGET): main.c $(OBJ)
	$(CC) $^ $(FLAGS) -o $@

%.o: %.c
	$(CC) -c $(FLAGS) $^ -o $@

clean:
	rm -rfv $(OBJ) $(TARGET)
