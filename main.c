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

#define COMPONENTS_MAX 1024
int main(void) {

	// TODO: Make this a dynamic-array
	Components components = {0};

	add_base_component(&components, CLITERAL(Vector2) { 100.f, 100.f });
	
	for (int i = 0; i < (int)components.count; ++i) {
		// todo: draw components
		Component *c = &components.data[i];
		printf("pos: %f, %f\n", c->pos.x, c->pos.y);
		printf("size: %f, %f\n", c->size.x, c->size.y);
	}

	InitWindow(WIDTH, HEIGHT, "Bit Game");
	
	float scl = 0.5f;
	RenderTexture2D ren_tex = LoadRenderTexture((int)(WIDTH*scl), (int)(HEIGHT*scl));
	if (!IsRenderTextureValid(ren_tex)) {
		log_error("Failed to create RenderTexture2D!");
	}
	log_info("Created RenderTexture2D with dimensions %dx%d", ren_tex.texture.width, ren_tex.texture.height);

	while (!WindowShouldClose()) {
		BeginDrawing();
		BeginTextureMode(ren_tex);

		ClearBackground(COLOR1);

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

	UnloadRenderTexture(ren_tex);
	CloseWindow();
	return 0;
}
