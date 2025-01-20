#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define ASSERT(cond, msg) do {\
		if (!(cond)) {\
			fprintf(stderr, "%s:%d:0 ASSERTION FAILED: %s, %s\n", __FILE__, __LINE__, #cond, msg);\
			exit(1);\
		}\
	} while (0)

#define log_error(fmt, ...) do {\
		fprintf(stderr, "%s "fmt"\n", "[ERROR]", ##__VA_ARGS__);\
	} while (0)
#define log_info(fmt, ...) do {\
		fprintf(stdout, "%s "fmt"\n", "[INFO]", ##__VA_ARGS__);\
	} while (0)
#define log_warning(fmt, ...) do {\
		fprintf(stdout, "%s "fmt"\n", "[WARNING]", ##__VA_ARGS__);\
	} while (0)

#define DYNAMIC_ARRAY_INITIAL_CAPACITY (sizeof(size_t))

#define da_append(da, elm) do {\
		if ((da).data == NULL) {\
			(da).capacity = DYNAMIC_ARRAY_INITIAL_CAPACITY;\
			(da).count = 0;\
			(da).data = malloc(sizeof(elm) * (da).capacity);\
		}\
		if ((da).count + 1 > (da).capacity) {\
			(da).capacity *= 2;\
			ASSERT(realloc((da).data, (da).capacity * sizeof(*(da).data)) != NULL, "TODO: Log error instead of asserting");\
		}\
		(da).data[(da).count++] = elm;\
	} while (0)

int _screen_width, _screen_height;
float _scl;

// Window
RenderTexture2D init_window(int screen_width, int screen_height, float scl, const char *title);
void close_window(RenderTexture2D ren_tex);

// Draw
void draw_ren_tex(RenderTexture2D ren_tex, int screen_width, int screen_height);

// Misc
Vector2 get_mpos_scaled(RenderTexture2D ren_tex);

#endif // _ENGINE_H_

// IMPLEMENTATION ////////////////////////////////
#ifdef ENGINE_IMPLEMENTATION

// Setup
RenderTexture2D init_window(int screen_width, int screen_height, float scl, const char *title) {
	SetTraceLogLevel(LOG_NONE);
	InitWindow(screen_width, screen_height, title);
	_scl = scl;
	_screen_width = screen_width;
	_screen_height = screen_height;

	log_info("Created Window with dimensions %dx%d", _screen_width, _screen_height);

	RenderTexture2D ren_tex = LoadRenderTexture((int)(screen_width*scl), (int)(screen_height*scl));
	if (!IsRenderTextureValid(ren_tex)) {
		log_error("Failed to create RenderTexture2D!");
		exit(1);
	}
	log_info("Created RenderTexture2D with dimensions %dx%d (Scaled down by %.2f)", ren_tex.texture.width, ren_tex.texture.height, _scl);

	return ren_tex;
}

void close_window(RenderTexture2D ren_tex) {
	UnloadRenderTexture(ren_tex);
	CloseWindow();
}

// Draw
void draw_ren_tex(RenderTexture2D ren_tex, int screen_width, int screen_height) {
	const Rectangle src = {
		.x = 0,
		.y = 0,
		.width = ren_tex.texture.width,
		// NOTE: We flip the height because y-axis is flipped internally (in opengl land probably)
		.height = -ren_tex.texture.height,
	};

	const Rectangle dst = {
		.x = 0,
		.y = 0,
		.width = screen_width,
		.height = screen_height,
	};
	DrawTexturePro(ren_tex.texture, src, dst, CLITERAL(Vector2) { 0.f, 0.f }, 0.f, WHITE);
}

// Misc
Vector2 get_mpos_scaled(RenderTexture2D ren_tex) {
	Vector2 m = GetMousePosition();
	m.x *= _scl;
	m.y *= _scl;
	return m;
}

#endif // ENGINE_IMPLEMENTATION
