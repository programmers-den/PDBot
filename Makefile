TARGET = main
CC = gcc
SRC = $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)
FLAGS = -g -ldiscord -lcurl -lcrypto -lssl -lm -lpthread -pthread -lsqlite3 -ljson-c
# FLAGS = -g -static -ldiscord -lcurl-bearssl -lbearssl -lm -lpthread -pthread -lsqlite3 -ljson-c

all: $(TARGET)

$(TARGET): main.c $(OBJ)
	$(CC) $^ $(FLAGS) -o $@

%.o: %.c
	$(CC) -c $(FLAGS) $^ -o $@

clean:
	rm -rfv $(OBJ) $(TARGET)
