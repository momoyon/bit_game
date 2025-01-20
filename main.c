#define ENGINE_IMPLEMENTATION
#include <engine.h>

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define SCL 0.5f
#define WIDTH  (SCREEN_WIDTH*SCL)
#define HEIGHT (SCREEN_HEIGHT*SCL)

#define COLOR1 GetColor(0x181818FF)
#define TILE_SIZE 16
#define CHUNK_TILE_COUNT 16

static bool DEBUG_DRAW = true;

Vector2 v2_aligned_to_tile(Vector2 v) {
	Vector2 res = {0};
	res.x = (float)(((int)v.x / (int)TILE_SIZE) * TILE_SIZE);
	res.y = (float)(((int)v.y / (int)TILE_SIZE) * TILE_SIZE);
	return res;
}

/// Debug
void debug_draw_world_grid(float size, Camera2D camera, Color color) {
	Vector2 world_top_left = GetScreenToWorld2D(CLITERAL(Vector2) { 0.f, 0.f }, camera);
	for (int y = -1; y < (HEIGHT/size)+1; ++y) {
		Vector2 s = { .x = (float)-size, .y = (float)y*size, };
		Vector2 e = { .x = (float)WIDTH + size, .y = (float)y*size, };
		s = v2_aligned_to_tile(Vector2Add(s, world_top_left));
		e = v2_aligned_to_tile(Vector2Add(e, world_top_left));
		DrawLineV(s, e, color);
	}
	for (int x = -1; x < (WIDTH/size)+1; ++x) {
		Vector2 s = { .x = (float)x*size, .y = (float)-size, };
		Vector2 e = { .x = (float)x*size, .y = (float)HEIGHT + size, };
		s = v2_aligned_to_tile(Vector2Add(s, world_top_left));
		e = v2_aligned_to_tile(Vector2Add(e, world_top_left));
		DrawLineV(s, e, color);
	}
}

typedef enum {
	COMP_TYPE_BASE,
	COMP_TYPE_COUNT,
} Component_type;

typedef struct {
	Component_type type;
	Vector2 pos;
	Vector2 size;
} Component;

Component make_component(Component_type type, const Vector2 pos) {
	Component res = {
		.type = type,
		.pos = pos,
		.size = CLITERAL(Vector2) { TILE_SIZE, TILE_SIZE }
	};
	switch (res.type) {
		case COMP_TYPE_BASE: {

				     } break;
	        case COMP_TYPE_COUNT:
	        default: ASSERT(0, "UNREACHABLE!");
	}
	return res;
}

void draw_component(Component* comp) {
	switch (comp->type) {
		case COMP_TYPE_BASE: {
					     DrawRectangleV(comp->pos, comp->size, RED);
				     } break;
	        case COMP_TYPE_COUNT:
	        default: ASSERT(0, "UNREACHABLE!");
	}
}

typedef struct {
	Component *data;
	size_t count;
	size_t capacity;
} Components;

void add_base_component(Components *components, Vector2 pos) {
	Component comp = make_component(COMP_TYPE_BASE, pos);
	da_append(*components, comp);
}

#define COMPONENTS_MAX 1024
int main(void) {

	Components components = {0};

	Vector2 mpos = {0};
	Camera2D camera = {
		.offset = CLITERAL(Vector2) { WIDTH * 0.5f, HEIGHT * 0.5f },
		.target = CLITERAL(Vector2) { WIDTH * 0.5f, HEIGHT * 0.5f },
		.rotation = 0.f,
		.zoom = 1.f,
	};
	float camera_move_speed = 100.f;

	RenderTexture2D ren_tex = init_window(SCREEN_WIDTH, SCREEN_HEIGHT, SCL, "Bit Game");

	while (!WindowShouldClose()) {
		BeginDrawing();
		BeginTextureMode(ren_tex);

		ClearBackground(COLOR1);
		float delta = GetFrameTime();

		mpos = get_mpos_scaled(ren_tex);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			add_base_component(&components, v2_aligned_to_tile(mpos));
		}
		// Update
		for (int i = 0; i < (int)components.count; ++i) {
			// TODO: update components
		}

		if (IsKeyDown(KEY_A)) {
			camera.target.x -= camera_move_speed * delta;
		}
		if (IsKeyDown(KEY_D)) {
			camera.target.x += camera_move_speed * delta;
		}
		if (IsKeyDown(KEY_W)) {
			camera.target.y -= camera_move_speed * delta;
		}
		if (IsKeyDown(KEY_S)) {
			camera.target.y += camera_move_speed * delta;
		}

		BeginMode2D(camera);
		// Draw
		for (int i = 0; i < (int)components.count; ++i) {
			// TODO: draw components
			Component *c = &components.data[i];
			draw_component(c);
		}



		if (DEBUG_DRAW) {
			// Draw origin of world
			DrawCircleV(CLITERAL(Vector2) { 0.f, 0.f }, 2.f, WHITE);
			debug_draw_world_grid(TILE_SIZE, camera, ColorAlpha(WHITE, 0.25f));
			// TODO: Chunk grid is drawn wrong
			debug_draw_world_grid(TILE_SIZE*CHUNK_TILE_COUNT, camera, ColorAlpha(WHITE, 1.f));

		}
		EndMode2D();

		EndTextureMode();
		draw_ren_tex(ren_tex, SCREEN_WIDTH, SCREEN_HEIGHT);
		EndDrawing();
	}

	close_window(ren_tex);
	return 0;
}
