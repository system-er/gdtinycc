#ifndef GDTINYCC_RUNTIME_H
#define GDTINYCC_RUNTIME_H



void godot_print(const char *format, ...);
void* godot_get_node(void* self, const char *path);
void* godot_get_parent(void* node_ptr);
const char* godot_get_property(void* node, const char *property);

typedef struct { float x, y; } tcc_Vector2;
typedef struct { float x, y, z; } tcc_Vector3;
typedef struct { int x, y; } tcc_Vector2i;
typedef struct { int x, y, z; } tcc_Vector3i;

typedef struct {
    float r, g, b, a;
} tcc_Color;

typedef struct {
    tcc_Vector2 position;
    tcc_Vector2 size;
} tcc_Rect2;

typedef struct {
    tcc_Vector2i position;
    tcc_Vector2i size;
} tcc_Rect2i;

typedef struct {
    int type;
    union {
        int i;
        float f;
        char s[256];
        int b;
        tcc_Vector2 vec2;
        tcc_Vector3 vec3;
        tcc_Vector2i vec2i;
        tcc_Vector3i vec3i;
        tcc_Color color;
        tcc_Rect2 rect2;
        tcc_Rect2i rect2i;
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
    VARTYPE_VECTOR2I = 6,
    VARTYPE_RECT2 = 7,
    VARTYPE_RECT2I = 8,
    VARTYPE_VECTOR3 = 9,
    VARTYPE_VECTOR3I = 10,

    VARTYPE_COLOR = 20,
    VARTYPE_STRING_NAME = 21,
    VARTYPE_NODE_PATH = 22,
    VARTYPE_RID = 23,
    VARTYPE_OBJECT = 24,
    VARTYPE_DICTIONARY = 27,
    VARTYPE_ARRAY = 28,
    VARTYPE_PACKED_BYTE_ARRAY = 29,

} GDExtensionVarType;

void godot_get_variant(void* node, const char *property, GDExtensionVariant *result);
void godot_set_variant(void* node, const char* property, GDExtensionVariant variant);
void godot_free_variant(GDExtensionVariant* variant);
void* godot_instantiate(void* self, const char* scene_path);
void* godot_get_tree(void* self);
void* godot_create(const char* class_name);
void godot_add_child(void* parent, void* child);
void godot_add_child_deferred(void* parent, void* child);
void godot_call(void* node_ptr, 
                const char* method_name, 
                int arg_count, 
                GDExtensionVariant* args,
                GDExtensionVariant* result);
//GDExtensionVariant godot_call(void* node, const char* method_name, int arg_count, GDExtensionVariant* args);
GDExtensionVariant godot_call_object(void* node, const char* method_name, int arg_count, GDExtensionVariant* args);
void godot_queue_free(void* node);
void godot_print_float(float f);
const char* godot_get_type_name(int type);
void godot_emit_signal(void* node, const char* signal_name, int arg_count, GDExtensionVariant* args);
void godot_connect(void* self, void* node, const char* signal_name, void* callback_func, void* user_data);
long godot_get_ticks_msec();
void godot_delay_msec(int msec);
float godot_randf();
int godot_randi();
float godot_randf_range(float a, float b);
int godot_randi_range(int a, int b);
void godot_randomize();
void godot_draw_rect(void* canvas_item_ptr, float x, float y, float w, float h,
                     float r, float g, float b, float a, int filled);
void godot_draw_circle(void* canvas_item_ptr, float x, float y, float radius,
                              float r, float g, float b, float a, int filled);
void godot_draw_line(void* canvas_item_ptr, float x1, float y1, float x2, float y2,
                     float r, float g, float b, float a, float thickness);
void* godot_get_drawingnode(void* self);
void* godot_get_drawingcanvas(void* self);
int godot_is_pressed(void* event);
int godot_eventcode(void* event_ptr);
GDExtensionVariant godot_get_global_mouse_position(void* self);
void* godot_load_resource(const char* path, const char* type_hint);
const char* godot_get_class_name(void* obj);
void* godot_get_rendering_server();
void* godot_get_physics_server_2d();
void* godot_get_physics_server_3d();
void* godot_get_engine();
void* godot_get_display_server();
void* godot_get_physics_server2D();
void* godot_get_physics_server3D();  
void* godot_get_os();
void godot_free_variant(GDExtensionVariant* variant);
int godot_check_collision(void* area, void* other);
int godot_check_collision_3d(void* area3d, void* other);
void godot_setup_collision_shape(void* collision_shape, const char* shape_type, float param1, float param2, float param3);
void godot_remove_child(void* parent, void* child);
void godot_remove_child_deferred(void* parent, void* child);
void* godot_get_children(void* parent);
void* godot_find_node(void* parent, const char* name, int recursive);

typedef void* Array;

double sin(double x);
double cos(double x);
double abs(double x);
double tan(double x);
double atan(double x);
double atan2(double x);
double sqrt(double x);
double pow(double x);
double floor(double x);
double ceil(double x);
double fabs(double x);
double fmod(double x);

int add(int a, int b);

#endif
