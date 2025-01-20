#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>

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

// Vector helpers
Vector2 v2xx(float v);

// Vector2i
typedef struct {
	int x, y;
} Vector2i;

Vector2i v2vi(Vector2 v);
bool v2i_equal(Vector2i a, Vector2i b);

int _screen_width, _screen_height;
float _scl;

// Window
RenderTexture2D init_window(int screen_width, int screen_height, float scl, const char *title);
void close_window(RenderTexture2D ren_tex);

typedef enum {
	TEXT_ALIGN_H_LEFT = 0,
	TEXT_ALIGN_H_CENTER,
	TEXT_ALIGN_H_RIGHT,
	TEXT_ALIGN_H_COUNT,
} Text_align_h;

// NOTE: Start enum at 10 to not conflict with Text_align_h
typedef enum {
	TEXT_ALIGN_V_TOP = 10,
	TEXT_ALIGN_V_CENTER,
	TEXT_ALIGN_V_BOTTOM,
	TEXT_ALIGN_V_COUNT,
} Text_align_v;

// Draw
void draw_ren_tex(RenderTexture2D ren_tex, int screen_width, int screen_height);
void draw_text_aligned(Font font, const char *text, Vector2 pos, int font_size, const Text_align_v align_v, const Text_align_h align_h, Color color);
void draw_text(Font font, const char *text, Vector2 pos, int font_size, Color color);

// Misc
Vector2 get_mpos_scaled();

#endif // _ENGINE_H_

// IMPLEMENTATION ////////////////////////////////
#ifdef ENGINE_IMPLEMENTATION

// Vector helpers
Vector2 v2xx(float v) { return CLITERAL(Vector2) { v, v }; }

// Vector2i
Vector2i v2vi(Vector2 v) { return CLITERAL(Vector2i) { (int)v.x, (int)v.y }; }

bool v2i_equal(Vector2i a, Vector2i b) {
	return a.x == b.x && a.y == b.y;
}

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

void draw_text_aligned(Font font, const char *text, Vector2 pos, int font_size, const Text_align_v align_v, const Text_align_h align_h, Color color) {
	Vector2 origin = {0};
	// RLAPI Vector2 MeasureTextEx(Font font, const char *text, float fontSize, float spacing);    // Measure string size for Font
	float spacing = 2.f;
	Vector2 text_size = MeasureTextEx(font, text, font_size, spacing);

	switch (align_h) {
		case TEXT_ALIGN_H_LEFT: {
		} break;
		case TEXT_ALIGN_H_CENTER: {
			origin.x = text_size.x * 0.5f;
		} break;
		case TEXT_ALIGN_H_RIGHT: {
			origin.x = text_size.x;
		} break;
		case TEXT_ALIGN_H_COUNT: {
		} break;
		default: ASSERT(false, "UNREACHABLE");
	}

	switch (align_v) {
		case TEXT_ALIGN_V_TOP: {
		} break;
		case TEXT_ALIGN_V_CENTER: {
			origin.y = text_size.y * 0.5f;
		} break;
		case TEXT_ALIGN_V_BOTTOM: {
			origin.y = text_size.y;
		} break;
		case TEXT_ALIGN_V_COUNT: {
		} break;
		default: ASSERT(false, "UNREACHABLE");
	}

	DrawTextPro(font, text, pos, origin, 0.f, font_size, spacing, color);
}

void draw_text(Font font, const char *text, Vector2 pos, int font_size, Color color) {
	draw_text_aligned(font, text, pos, font_size, TEXT_ALIGN_V_TOP, TEXT_ALIGN_H_LEFT, color);
}

// Misc
Vector2 get_mpos_scaled() {
	Vector2 m = GetMousePosition();
	m.x *= _scl;
	m.y *= _scl;
	return m;
}

#endif // ENGINE_IMPLEMENTATION
