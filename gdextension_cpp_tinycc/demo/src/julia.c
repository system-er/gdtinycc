// Julia Set Zoom Demo - rendered in C via TinyCC
// Shows raw computation speed: ~480.000 pixel iterations per frame
#include "gdtinycc_runtime.h"

int snprintf(char* str, int size, const char* format, ...);

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WIDTH 800
#define HEIGHT 600

static float julia_cx = -0.7269f;
static float julia_cy = 0.1889f;

static float view_center_x = 0.0f;
static float view_center_y = 0.0f;
static float view_size = 3.5f;
static int max_iter = 150;

static float color_hue_shift = 0.0f;
static long start_ticks = 0;
static int frame_count = 0;
static int fps = 0;
static long fps_timer = 0;

static void hsv_to_rgb(float h, float s, float v, float* r, float* g, float* b) {
    if (s < 0.0f) s = 0.0f;
    if (s > 1.0f) s = 1.0f;
    if (v < 0.0f) v = 0.0f;
    if (v > 1.0f) v = 1.0f;

    int i = (int)(h * 6.0f);
    float f = h * 6.0f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));
    switch (i % 6) {
        case 0: *r = v; *g = t; *b = p; break;
        case 1: *r = q; *g = v; *b = p; break;
        case 2: *r = p; *g = v; *b = t; break;
        case 3: *r = p; *g = q; *b = v; break;
        case 4: *r = t; *g = p; *b = v; break;
        case 5: *r = v; *g = p; *b = q; break;
    }
}

static int julia_iter(float px, float py, float cx, float cy, int max_i) {
    float x = px;
    float y = py;
    int i = 0;
    float x2 = x * x;
    float y2 = y * y;
    while (x2 + y2 < 4.0f && i < max_i) {
        y = 2.0f * x * y + cy;
        x = x2 - y2 + cx;
        x2 = x * x;
        y2 = y * y;
        i++;
    }
    return i;
}

void main() {
    godot_print("Julia Set Zoom Demo - C speed test");
}

void _ready(void* self) {
    godot_randomize();
    start_ticks = godot_get_ticks_msec();
    fps_timer = start_ticks;

    float angle = godot_randf() * (float)(2.0 * M_PI);
    julia_cx = 0.7885f * (float)cos(angle);
    julia_cy = 0.7885f * (float)sin(angle);

    godot_print("Julia c = (%.4f, %.4f)", julia_cx, julia_cy);
}

void _draw(void* self) {
    void* drawingnode = godot_get_drawingnode(self);
    if (!drawingnode) return;

    float time_delta = (float)(godot_get_ticks_msec() - start_ticks) / 1000.0f;
    color_hue_shift = time_delta * 0.15f;

    view_size *= 0.99f;
    if (view_size < 0.0001f) {
        view_size = 3.5f;
        float angle = godot_randf() * (float)(2.0 * M_PI);
        julia_cx = 0.7885f * (float)cos(angle);
        julia_cy = 0.7885f * (float)sin(angle);
        godot_print("Reset - new Julia c = (%.4f, %.4f)", julia_cx, julia_cy);
    }

    float zoom_factor = 3.5f / view_size;
    max_iter = 150 + (int)(zoom_factor * 10.0f);
    if (max_iter > 500) max_iter = 500;

    float aspect = (float)WIDTH / (float)HEIGHT;
    float x_range = view_size * aspect;
    float y_range = view_size;

    frame_count++;
    long now = godot_get_ticks_msec();
    if (now - fps_timer >= 1000) {
        fps = frame_count;
        frame_count = 0;
        fps_timer = now;
    }

    for (int py = 0; py < HEIGHT; py++) {
        for (int px = 0; px < WIDTH; px++) {
            float cx = view_center_x + ((float)px / (float)WIDTH - 0.5f) * x_range;
            float cy = view_center_y + ((float)py / (float)HEIGHT - 0.5f) * y_range;

            int iter = julia_iter(cx, cy, julia_cx, julia_cy, max_iter);

            if (iter >= max_iter) {
                godot_draw_rect(drawingnode, (float)px, (float)py, 1.0f, 1.0f,
                                0.02f, 0.02f, 0.06f, 1.0f, 1);
            } else {
                // Each iteration gets a distinct hue - ~5 iterations per color cycle
                float h = (float)iter * 0.2f + color_hue_shift;
                h = h - (float)(int)h;
                float s = 0.9f;
                float v = 0.9f;

                float r, g, b;
                hsv_to_rgb(h, s, v, &r, &g, &b);

                godot_draw_rect(drawingnode, (float)px, (float)py, 1.0f, 1.0f,
                                r, g, b, 1.0f, 1);
            }
        }
    }

    char fps_text[64];
    snprintf(fps_text, 64, "FPS: %d | Zoom: %.1fx | Iter: %d", fps, zoom_factor, max_iter);
    godot_draw_string(drawingnode, "default_font", 10.0f, 20.0f, fps_text,
                      1.0f, 1.0f, 1.0f, 1.0f, 18.0f);
}

void _input(void* self, void* event) {
    if (!event) return;
    if (godot_is_pressed(event)) {
        int code = godot_eventcode(event);
        if (code == KEY_ESCAPE) {
            GDExtensionVariant result;
            void* tree = godot_get_tree(self);
            godot_call(tree, "quit", 0, NULL, &result);
        }
        else if (code == KEY_SPACE) {
            view_size = 3.5f;
            float angle = godot_randf() * (float)(2.0 * M_PI);
            julia_cx = 0.7885f * (float)cos(angle);
            julia_cy = 0.7885f * (float)sin(angle);
            godot_print("Reset - new Julia c = (%.4f, %.4f)", julia_cx, julia_cy);
        }
    }
}
