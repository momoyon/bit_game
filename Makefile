CC=gcc
CFLAGS=-Wall -Wextra -ggdb -Wswitch-enum
LIBS=-lraylib -lm

main: main.c
	$(CC) $(CFLAGS) -I./ -I./include -o $@ $< -L./ $(LIBS)

clean:
	rm main.exe 2>/dev/null
