#define ENGINE_IMPLEMENTATION
#include <engine.h>

#define WIDTH  800
#define HEIGHT 600

#define COLOR1 GetColor(0x181818FF)
#define TILE_SIZE 16

static bool DEBUG_DRAW = true;

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

Vector2 v2_aligned_to_tile(Vector2 v) {
	Vector2 res = {0};
	res.x = (float)(((int)v.x / (int)TILE_SIZE) * TILE_SIZE);
	res.y = (float)(((int)v.y / (int)TILE_SIZE) * TILE_SIZE);
	return res;
}

#define COMPONENTS_MAX 1024
int main(void) {

	Components components = {0};

	Vector2 mpos = {0};

	RenderTexture2D ren_tex = init_window(WIDTH, HEIGHT, 0.5f, "Bit Game");

	while (!WindowShouldClose()) {
		BeginDrawing();
		BeginTextureMode(ren_tex);

		ClearBackground(COLOR1);

		mpos = get_mpos_scaled(ren_tex);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			add_base_component(&components, v2_aligned_to_tile(mpos));
		}
		// Update
		for (int i = 0; i < (int)components.count; ++i) {
			// TODO: update components
		}

		// Draw
		for (int i = 0; i < (int)components.count; ++i) {
			// TODO: draw components
			Component *c = &components.data[i];
			draw_component(c);
		}

		EndTextureMode();
		draw_ren_tex(ren_tex, WIDTH, HEIGHT);
		EndDrawing();
	}

	close_window(ren_tex);
	return 0;
}
