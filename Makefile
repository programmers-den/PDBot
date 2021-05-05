target = main
compiler = gcc
src = $(wildcard src/*.c)
obj = $(src:.c=.o)
flags = -g -O3 -l curl -l crypto -l ssl -l m -l discord -l pthread -pthread
all: $(target)

$(target): main.c $(obj)
	$(compiler) $^ $(flags) -o $@

%.o: %.c
	$(compiler) -c $(flags) $^ -o $@

clean:
	rm -rfv src/*.o $(target)
