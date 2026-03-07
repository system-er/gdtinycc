#ifndef GDTINYCC_RUNTIME_H
#define GDTINYCC_RUNTIME_H

void godot_print(const char *msg);
void* godot_get_node(const char *path);
const char* godot_get_property(void* node, const char *property);

typedef struct {
    int type;
    union {
        int i;
        float f;
        char s[256];
        int b;
        float vec2[2];
        float vec3[3];
    } value;
} GDExtensionVariant;

GDExtensionVariant godot_get_variant(void* node, const char* property);
void godot_print_float(float f);
const char* godot_get_type_name(int type);

#endif
