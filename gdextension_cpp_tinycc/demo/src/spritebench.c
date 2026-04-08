// a sprite benchmark from https://github.com/timklge/spritebench
#include "gdtinycc_runtime.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SPRITE_COUNT 20000
#define FRAME_COUNT 1000
#define START_FRAME 100

typedef struct {
    void* sprite;
    float angle;
    float speed;
    float pos_x;
    float pos_y;
    float window_width;
    float window_height;
} SpriteData;

static SpriteData sprites[SPRITE_COUNT];
static int sprite_count = 0;
static float frame_times[FRAME_COUNT];
static int current_frame = 0;
static int frame_index = 0;
static int benchmark_done = 0;
static void* icon_texture = NULL;

void _ready(void* self) {
    godot_randomize();
    
    godot_print("Loading icon...");
    icon_texture = godot_load_resource("res://icon.svg", "Texture2D");
    if (!icon_texture) {
        godot_print("ERROR: Could not load icon.svg");
        return;
    }
    
    godot_print("Creating %d sprites...", SPRITE_COUNT);
    
    for (int i = 0; i < SPRITE_COUNT; i++) {
        void* sprite = godot_create("Sprite2D");
        if (!sprite) continue;
        
        sprites[i].sprite = sprite;
        sprites[i].angle = godot_randf_range(0.0f, (float)(M_PI * 2.0));
        sprites[i].speed = godot_randf_range(100.0f, 600.0f);
        sprites[i].pos_x = 640.0f;
        sprites[i].pos_y = 360.0f;
        sprites[i].window_width = 1280.0f;
        sprites[i].window_height = 720.0f;
        
        GDExtensionVariant tex_var;
        tex_var.type = 24;
        tex_var.ptr = icon_texture;
        godot_set_variant(sprite, "texture", tex_var);
        
        GDExtensionVariant pos_var;
        pos_var.type = 5;
        pos_var.value.vec2.x = sprites[i].pos_x;
        pos_var.value.vec2.y = sprites[i].pos_y;
        godot_set_variant(sprite, "position", pos_var);
        
        godot_add_child(self, sprite);
        sprite_count++;
        
        if ((i + 1) % 5000 == 0) {
            godot_print("Created %d sprites...", i + 1);
        }
    }
    
    godot_print("All %d sprites created", sprite_count);
}

void _process(void* self, double delta) {
    if (sprite_count == 0 || benchmark_done) return;
    
    current_frame++;
    float dt = (float)delta;
    
    if (current_frame >= START_FRAME && frame_index < FRAME_COUNT) {
        frame_times[frame_index] = dt;
        frame_index++;
        
        if (frame_index >= FRAME_COUNT) {
            benchmark_done = 1;
            
            for (int i = 0; i < sprite_count; i++) {
                godot_queue_free(sprites[i].sprite);
            }
            
            float total = 0.0f;
            for (int i = 0; i < FRAME_COUNT; i++) {
                total += frame_times[i];
            }
            float avg = total / FRAME_COUNT;
            godot_print("BENCHMARK DONE: avg frame time = %f ms", avg * 1000.0f);
            godot_print("original benchmark: https://github.com/timklge/spritebench");
        }
    }
    
    for (int i = 0; i < sprite_count; i++) {
        SpriteData* s = &sprites[i];
        
        float cos_a = (float)cos(s->angle);
        float sin_a = (float)sin(s->angle);
        s->pos_x += cos_a * s->speed * dt;
        s->pos_y += sin_a * s->speed * dt;
        
        if (s->pos_x < 32.0f || s->pos_x > s->window_width - 32.0f) {
            s->angle = (float)M_PI - s->angle;
        }
        if (s->pos_y < 32.0f || s->pos_y > s->window_height - 32.0f) {
            s->angle = -s->angle;
        }
        
        GDExtensionVariant pos_var;
        pos_var.type = 5;
        pos_var.value.vec2.x = s->pos_x;
        pos_var.value.vec2.y = s->pos_y;
        godot_set_variant(s->sprite, "position", pos_var);
    }
}