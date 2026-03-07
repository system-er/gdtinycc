#ifndef GDTINYCC_RUNTIME_H
#define GDTINYCC_RUNTIME_H

void godot_print(const char *msg);
void* godot_get_node(const char *path);
const char* godot_get_property(void* node, const char *property);

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} Color;

typedef struct {
    int type;
    union {
        int i;
        float f;
        char s[256];
        int b;
        Vector2 vec2;
        Vector3 vec3;
        Color color;
    } value;
} GDExtensionVariant;

GDExtensionVariant godot_get_variant(void* node, const char* property);
void godot_print_float(float f);
const char* godot_get_type_name(int type);

#endif
