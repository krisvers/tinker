CC = gcc
FLAGS = -O3 -lm
EXTRA = -lSDL2

all:
	$(CC) main.c -o tinker $(FLAGS) $(EXTRA)
