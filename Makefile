CC := gcc
CFLAGS := -Isrc/include -lSDL2 -std=c99
SRC := $(shell find ./src -type f -name "*.c")
OBJ := $(SRC:.c=.o)

all: clean $(OBJ)
	$(CC) $(shell find ./obj -type f -name "*.o") -o game $(CFLAGS)

tool:
	$(CC) ./tools/src/kmg.c -o ./tools/kmg -Isrc/include -std=c99

clean:
	rm -rf obj/*
	mkdir -p obj/src/

%.o: %.c
	$(CC) -c $< -o obj/$@ $(CFLAGS)
