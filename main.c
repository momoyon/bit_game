#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define ASSERT(cond, msg) do {\
		if (!(cond)) {\
			fprintf(stderr, "ASSERTION FAILED: %s, %s\n", #cond, msg);\
			exit(1);\
		}\
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
			ASSERT(realloc((da).data, (da).capacity) != NULL, "TODO: Log error instead of asserting");\
		}\
		(da).data[(da).count++] = elm;\
	} while (0)

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
	while (!WindowShouldClose()) {
		BeginDrawing();
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

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
