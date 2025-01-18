CC=gcc
CFLAGS=-Wall -Wextra -ggdb
LIBS=-l:raylib.dll

main: main.c
	$(CC) $(CFLAGS) -I./raylib-5.5_win64_mingw-w64/include -I./ -o $@ $< -L./ $(LIBS)

clean:
	rm main.exe 2>/dev/null
