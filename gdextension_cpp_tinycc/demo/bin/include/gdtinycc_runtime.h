#ifndef GDTINYCC_RUNTIME_H
#define GDTINYCC_RUNTIME_H

void godot_print(const char *msg);
void* godot_get_node(const char *path);
void* godot_get_parent(void* node_ptr);
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

typedef enum {
    VARTYPE_NULL = 0,
    VARTYPE_BOOL = 1,
    VARTYPE_INT = 2,
    VARTYPE_FLOAT = 3,
    VARTYPE_STRING = 4,
    VARTYPE_VECTOR2 = 5,
    VARTYPE_VECTOR3 = 6,
    VARTYPE_COLOR = 7
} GDExtensionVarType;

GDExtensionVariant godot_get_variant(void* node, const char* property);
void godot_set_variant(void* node, const char* property, GDExtensionVariant variant);
void* godot_instantiate(const char* scene_path);
void* godot_create(const char* class_name);
void godot_add_child(void* parent, void* child);
void godot_add_child_deferred(void* parent, void* child);
GDExtensionVariant godot_call(void* node, const char* method_name, int arg_count, GDExtensionVariant* args);
GDExtensionVariant godot_call_object(void* node, const char* method_name, int arg_count, GDExtensionVariant* args);
void godot_queue_free(void* node);
void godot_print_float(float f);
const char* godot_get_type_name(int type);
void godot_emit_signal(void* node, const char* signal_name, int arg_count, GDExtensionVariant* args);
void godot_connect(void* node, const char* signal_name, void* callback_func, void* user_data);
long godot_get_ticks_msec();
float godot_randf();
int godot_randi();
float godot_randf_range(float a, float b);
int godot_randi_range(int a, int b);
void godot_randomize();
void godot_draw_rect(void* canvas_item_ptr, float x, float y, float w, float h,
                     float r, float g, float b, float a, int filled);

#endif
