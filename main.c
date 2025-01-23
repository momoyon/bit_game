#define ENGINE_IMPLEMENTATION
#include <engine.h>

int screen_width  = 1280*0.5;
int screen_height = 720*0.5;

#define SCL 0.5f
int width = 0;
int height = 0;


#define COLOR1 GetColor(0x181818FF)
#define TILE_SIZE 16
#define CHUNK_TILE_COUNT 16
#define MAX_CHUNK_COUNT 4

static bool DEBUG_DRAW = true;

Vector2 v2_aligned_to_by(Vector2 v, float by) {
	Vector2 res = {0};
	res.x = (float)(((int)v.x / (int)by) * by);
	res.y = (float)(((int)v.y / (int)by) * by);
	return res;
}

Vector2 v2_aligned_to_tile(Vector2 v) { return v2_aligned_to_by(v, TILE_SIZE); }
Vector2 v2_aligned_to_chunk(Vector2 v) { return v2_aligned_to_by(v, TILE_SIZE*CHUNK_TILE_COUNT); }

/// Debug
void debug_draw_world_grid(float size, Camera2D camera, Color color) {
	Vector2 world_top_left = GetScreenToWorld2D(v2( 0.f, 0.f ), camera);
	Vector2 world_bottom_right = GetScreenToWorld2D(v2( width, height ), camera);
	Vector2i view_size = {
		.x = world_bottom_right.x - world_top_left.x,
		.y = world_bottom_right.y - world_top_left.y,
	};

	for (int y = -1; y < (view_size.y/size)+1; ++y) {
		Vector2 s = { .x = (float)-size, .y = (float)y*size, };
		Vector2 e = { .x = (float)view_size.x + size, .y = (float)y*size, };
		s = v2_aligned_to_by(Vector2Add(s, world_top_left), size);
		e = v2_aligned_to_by(Vector2Add(e, world_top_left), size);
		DrawLineV(s, e, color);
	}

	for (int x = -1; x < (view_size.x/size)+1; ++x) {
		Vector2 s = { .x = (float)x*size, .y = (float)-size, };
		Vector2 e = { .x = (float)x*size, .y = (float)view_size.y + size, };
		s = v2_aligned_to_by(Vector2Add(s, world_top_left), size);
		e = v2_aligned_to_by(Vector2Add(e, world_top_left), size);
		DrawLineV(s, e, color);
	}
}


typedef enum {
	COMP_TYPE_BASE,
	COMP_TYPE_DISPLAY,
	COMP_TYPE_OUTPUT,
	COMP_TYPE_COUNT,
} Component_type;

const char *component_type_as_str(const Component_type type) {
    switch(type) {
        case COMP_TYPE_BASE: return "Base";
        case COMP_TYPE_DISPLAY: return "Display";
        case COMP_TYPE_OUTPUT: return "Output";
        case COMP_TYPE_COUNT: 
        default: ASSERT(false, "UNREACHABLE!");
    }
    return "INVALID";
}

typedef struct {
	Component_type type;
	Vector2 pos;
	Vector2 size;
	Vector2i chunk_id; // Chunk id in the world
	Vector2i tile_id;  // Tile id in the chunk

	int value; // Display value for DISPLAY; Expected value for OUTPUT;
} Component;

Component make_component(Component_type type, const Vector2 pos) {
	Component res = {
		.type = type,
		.pos = pos,
		.size = v2( TILE_SIZE, TILE_SIZE )
	};

	// Calculate chunk and tile id based on position
	res.chunk_id = v2vi(Vector2Divide(pos, v2xx(CHUNK_TILE_COUNT*TILE_SIZE)));
	res.tile_id = v2vi(Vector2Divide(pos, v2xx(TILE_SIZE)));
	res.tile_id.x = res.tile_id.x % CHUNK_TILE_COUNT;
	res.tile_id.y = res.tile_id.y % CHUNK_TILE_COUNT;
	/*log_info("Added component at chunk %d, %d", res.chunk_id.x, res.chunk_id.y);*/
        /*log_info("Added component at tile %d, %d", res.tile_id.x, res.tile_id.y);*/
	switch (res.type) {
		case COMP_TYPE_BASE: {
		} break;
		case COMP_TYPE_DISPLAY: {
		} break;
		case COMP_TYPE_OUTPUT: {
		} break;
	        case COMP_TYPE_COUNT:
	        default: ASSERT(0, "UNREACHABLE!");
	}

	return res;
}

void draw_component(Component* comp) {
	switch (comp->type) {
		case COMP_TYPE_BASE: {
			DrawRectangleV(comp->pos, comp->size, GRAY);
			draw_text_aligned(GetFontDefault(), "C", Vector2Add(comp->pos, Vector2Scale(comp->size, 0.5f)), TILE_SIZE, TEXT_ALIGN_V_CENTER, TEXT_ALIGN_H_CENTER, WHITE);
		} break;
		case COMP_TYPE_DISPLAY: {
			DrawRectangleV(comp->pos, comp->size, ORANGE);
			const char *value_str = tprintf("%d", comp->value);
			draw_text_aligned(GetFontDefault(), value_str, Vector2Add(comp->pos, Vector2Scale(comp->size, 0.5f)), TILE_SIZE, TEXT_ALIGN_V_CENTER, TEXT_ALIGN_H_CENTER, WHITE);
		} break;
		case COMP_TYPE_OUTPUT: {
			DrawRectangleV(comp->pos, comp->size, RED);
			const char *value_str = tprintf("%d", comp->value);
			draw_text_aligned(GetFontDefault(), value_str, Vector2Add(comp->pos, Vector2Scale(comp->size, 0.5f)), TILE_SIZE, TEXT_ALIGN_V_CENTER, TEXT_ALIGN_H_CENTER, WHITE);
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
	/*log_info("Added component [%zu]", components->count);*/
	da_append(*components, comp);
}

void add_display_component(Components *components, Vector2 pos) {
	Component comp = make_component(COMP_TYPE_DISPLAY, pos);
	/*log_info("Added component [%zu]", components->count);*/
	da_append(*components, comp);
}

void add_output_component(Components *components, Vector2 pos, int expected_value) {
	Component comp = make_component(COMP_TYPE_OUTPUT, pos);
	comp.value = expected_value;
	da_append(*components, comp);
}


bool component_exists_at(Components *components, Vector2 pos) {
	Vector2i chunk_id = v2vi(Vector2Divide(pos, v2xx(CHUNK_TILE_COUNT*TILE_SIZE)));
	Vector2i tile_id = v2vi(Vector2Divide(pos, v2xx(TILE_SIZE)));
	tile_id.x = tile_id.x % CHUNK_TILE_COUNT;
	tile_id.y = tile_id.y % CHUNK_TILE_COUNT;
	for (int i = 0; i < (int)components->count; ++i) {
		if (v2i_equal(components->data[i].chunk_id, chunk_id) && v2i_equal(components->data[i].tile_id, tile_id)) {
			return true;
		}
	}

	return false;
}

int main(void) {
	width = screen_width*SCL;
	height = screen_height*SCL;
	DEBUG = true;
	size_t max_components_count = MAX_CHUNK_COUNT*(CHUNK_TILE_COUNT*CHUNK_TILE_COUNT);
	log_info("Max components count: %zu", max_components_count);
	Components components = {
		.data = malloc(sizeof(Component)*max_components_count),
		.count = 0,
		.capacity = max_components_count,
	};

	Components components_next = {
		.data = malloc(sizeof(Component)*max_components_count),
	};

	Vector2 mpos = {0};
	Camera2D camera = {
		.offset = v2( width * 0.5f, height * 0.5f ),
		.target = v2( width * 0.5f, height * 0.5f ),
		.rotation = 0.f,
		.zoom = 1.f,
	};

	float camera_move_speed = 200.f;

	RenderTexture2D ren_tex = init_window(screen_width, screen_height, SCL, "Bit Game");

	SetWindowState(FLAG_WINDOW_RESIZABLE);
	// Stats
	int turn = 0;
	int bit = 0;

	Component current_selected_comp = make_component(COMP_TYPE_BASE, v2xx(0.0));
    Component hovering_comp = make_component(COMP_TYPE_BASE, v2xx(0.f));
    int current_output_component_expected_value = 4;

	Font font = GetFontDefault();
	while (!WindowShouldClose()) {
		if (IsWindowResized()) {
			UnloadRenderTexture(ren_tex);
			screen_width = GetScreenWidth();
			screen_height = GetScreenHeight();
			width = screen_width*SCL;
			height = screen_height*SCL;
			// log_info("Resized SCREEN: %dx%d RENDER: %dx%d", screen_width, screen_height, width, height);
			ren_tex = LoadRenderTexture(width, height);
		}

		temp_buff.count = 0;
		BeginDrawing();
		BeginTextureMode(ren_tex);

		ClearBackground(COLOR1);
		float delta = GetFrameTime();

		mpos = get_mpos_scaled();
		/*// Camera zoom*/
		/*camera.zoom = Clamp(camera.zoom + GetMouseWheelMove() * delta * 10.f, 0.5f, 5.f);*/
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !component_exists_at(&components, v2_aligned_to_tile(GetScreenToWorld2D(mpos, camera)))) {
            log_info("Added %s component!", component_type_as_str(current_selected_comp.type));
            switch (current_selected_comp.type) {
                case COMP_TYPE_BASE: add_base_component(&components, v2_aligned_to_tile(GetScreenToWorld2D(mpos, camera))); break;
                case COMP_TYPE_DISPLAY: add_display_component(&components, v2_aligned_to_tile(GetScreenToWorld2D(mpos, camera))); break;
                case COMP_TYPE_OUTPUT: add_output_component(&components, v2_aligned_to_tile(GetScreenToWorld2D(mpos, camera)), current_output_component_expected_value); break;
                case COMP_TYPE_COUNT: 
                default: ASSERT(false, "UNREACHABLE!");
            }
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

        // Switch current component type
        if (IsKeyPressed(KEY_Q)) {
            current_selected_comp.type = current_selected_comp.type <= 0 ? COMP_TYPE_COUNT-1 : current_selected_comp.type-1;
        }
        if (IsKeyPressed(KEY_E)) {
            current_selected_comp.type = current_selected_comp.type >= COMP_TYPE_COUNT-1 ? 0 : current_selected_comp.type+1;
        }

		// Clamp camera to positive axis.
		camera.target.x = Clamp(camera.target.x, camera.offset.x-1, (MAX_CHUNK_COUNT*CHUNK_TILE_COUNT*TILE_SIZE)-camera.offset.x);
		camera.target.y = Clamp(camera.target.y, camera.offset.y, (MAX_CHUNK_COUNT*CHUNK_TILE_COUNT*TILE_SIZE)-camera.offset.y+1);
		BeginMode2D(camera);

		// Draw
		for (int i = 0; i < (int)components.count; ++i) {
			// TODO: draw components
			Component *c = &components.data[i];
			draw_component(c);
		}


		if (DEBUG_DRAW) {
			debug_draw_world_grid(TILE_SIZE, camera, ColorAlpha(WHITE, 0.25f));
			debug_draw_world_grid(TILE_SIZE*CHUNK_TILE_COUNT, camera, ColorAlpha(WHITE, 1.f));
			// Draw origin of world
			DrawCircleV(v2xx(0.f), 2.f, RED);
		}

		EndMode2D();

		// Draw stats
		Vector2 top_right = v2(width, 0.f);
		const char *bit_str = tprintf("Bits: %08d", bit);
		draw_text_aligned(font, bit_str, top_right, TILE_SIZE, TEXT_ALIGN_V_TOP, TEXT_ALIGN_H_RIGHT, WHITE);

		const char *turn_str = tprintf("Turn: %10d", turn);
		Vector2 p = Vector2Add(top_right, v2(0, TILE_SIZE));
		draw_text_aligned(font, turn_str, p, TILE_SIZE, TEXT_ALIGN_V_TOP, TEXT_ALIGN_H_RIGHT, WHITE);

        const char *current_selected_comp_str = tprintf("Comp: %s", component_type_as_str(current_selected_comp.type));
        p = Vector2Add(top_right, v2(0, TILE_SIZE*2));
        draw_text_aligned(font, current_selected_comp_str, p, TILE_SIZE, TEXT_ALIGN_V_TOP, TEXT_ALIGN_H_RIGHT, YELLOW);

		EndTextureMode();
		draw_ren_tex(ren_tex, screen_width, screen_height);
		EndDrawing();
	}

	close_window(ren_tex);
	return 0;
}

