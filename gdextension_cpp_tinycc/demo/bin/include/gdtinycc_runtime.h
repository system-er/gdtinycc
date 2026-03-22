#ifndef GDTINYCC_RUNTIME_H
#define GDTINYCC_RUNTIME_H



void godot_print(const char *format, ...);
void* godot_get_node(void* self, const char *path);
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
    int x;
    int y;
} Vector2i;

typedef struct {
    int x;
    int y;
    int z;
} Vector3i;

typedef struct {
    Vector2 end;
    Vector2 position;
    Vector2 size;
} Rect2;

typedef struct {
    Vector2i end;
    Vector2i position;
    Vector2i size;
} Rect2i;


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
        Vector2i vec2i;
        Vector3i vec3i;
        Color color;
        Rect2 rect2;
        Rect2i rect2i;
    } value;

    void* ptr;

} GDExtensionVariant;

typedef enum {
    VARTYPE_NULL = 0,
    VARTYPE_BOOL = 1,
    VARTYPE_INT = 2,
    VARTYPE_FLOAT = 3,
    VARTYPE_STRING = 4,
    VARTYPE_VECTOR2 = 5,
    VARTYPE_VECTOR3 = 6,
    VARTYPE_COLOR = 7,
    VARTYPE_RECT2 = 8,

    VARTYPE_STRING_NAME = 21,
    VARTYPE_OBJECT = 24,
    VARTYPE_DICTIONARY = 27,
    VARTYPE_ARRAY = 28,
    VARTYPE_VECTOR2I = 20,
    VARTYPE_VECTOR3I = 22,
    VARTYPE_PACKED_BYTE_ARRAY = 23,
    VARTYPE_RECT2I = 19,

} GDExtensionVarType;

GDExtensionVariant godot_get_variant(void* node, const char* property);
void godot_set_variant(void* node, const char* property, GDExtensionVariant variant);
void* godot_instantiate(void* self, const char* scene_path);
void* godot_create(const char* class_name);
void godot_add_child(void* parent, void* child);
void godot_add_child_deferred(void* parent, void* child);
GDExtensionVariant godot_call(void* node, const char* method_name, int arg_count, GDExtensionVariant* args);
GDExtensionVariant godot_call_object(void* node, const char* method_name, int arg_count, GDExtensionVariant* args);
void godot_queue_free(void* node);
void godot_print_float(float f);
const char* godot_get_type_name(int type);
void godot_emit_signal(void* node, const char* signal_name, int arg_count, GDExtensionVariant* args);
void godot_connect(void* self, void* node, const char* signal_name, void* callback_func, void* user_data);
long godot_get_ticks_msec();
float godot_randf();
int godot_randi();
float godot_randf_range(float a, float b);
int godot_randi_range(int a, int b);
void godot_randomize();
void godot_draw_rect(void* canvas_item_ptr, float x, float y, float w, float h,
                     float r, float g, float b, float a, int filled);
void godot_draw_circle(void* canvas_item_ptr, float x, float y, float radius,
                              float r, float g, float b, float a, int filled);
void* godot_get_drawingnode();
int godot_is_pressed(void* event);
int godot_eventcode(void* event_ptr);
GDExtensionVariant godot_get_global_mouse_position(void* self);
void* godot_load_resource(const char* path, const char* type_hint);
const char* godot_get_class_name(void* obj);
double sin(double x);
double cos(double x);

int add(int a, int b);

#endif
