#include <stdio.h>
#include <raylib.h>

#define WIDTH  800
#define HEIGHT 600

#define COLOR1 GetColor(0x181818FF)

int main(void) {
	InitWindow(WIDTH, HEIGHT, "Bit Game");
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(COLOR1);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
