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
    //VARTYPE_PACKED_VECTOR3_ARRAY = 36,

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
void godot_call_pod(void* pod_ptr, 
                    int type_id,
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
void godot_draw_string(void* canvas_item_ptr, const char* font, float x, float y, const char* text,
                       float r, float g, float b, float a, float font_size);
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
void* godot_get_input();
void godot_free_variant(GDExtensionVariant* variant);
int godot_check_collision(void* area, void* other);
int godot_check_collision_3d(void* area3d, void* other);
void godot_setup_collision_shape(void* collision_shape, const char* shape_type, float param1, float param2, float param3);
void godot_remove_child(void* parent, void* child);
void godot_remove_child_deferred(void* parent, void* child);
int godot_get_children_count(void* parent);
void* godot_get_child_at(void* parent, int index);
void* godot_find_node(void* parent, const char* name, int recursive);
void godot_call_deferred(void* node_ptr, const char* method_name, int arg_count, GDExtensionVariant* args);
void* godot_file_open(const char* path, const char* mode);
int godot_file_read(void* handle, char* buffer, int size);
int godot_file_write(void* handle, const char* buffer, int size);
void godot_file_close(void* handle);
void godot_file_seek(void* handle, long position);
long godot_file_get_position(void* handle);
int godot_file_eof(void* handle);
long godot_file_get_size(void* handle);
int godot_file_exists(const char* path);
int godot_directory_exists(const char* path);
int godot_make_dir(const char* path);
int godot_remove_file(const char* path);
int godot_remove_dir(const char* path);
float godot_clamp_float(float value, float min_val, float max_val);
float godot_lerp_float(float from, float to, float weight);
float godot_lerp_angle(float from, float to, float weight);
int godot_clamp_int(int value, int min_val, int max_val);
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

// codes from godot_cpp/classes/global_constants.hpp
enum Key {
    KEY_NONE = 0,
    KEY_SPECIAL = 4194304,
    KEY_ESCAPE = 4194305,
    KEY_TAB = 4194306,
    KEY_BACKTAB = 4194307,
    KEY_BACKSPACE = 4194308,
    KEY_ENTER = 4194309,
    KEY_KP_ENTER = 4194310,
    KEY_INSERT = 4194311,
    KEY_DELETE = 4194312,
    KEY_PAUSE = 4194313,
    KEY_PRINT = 4194314,
    KEY_SYSREQ = 4194315,
    KEY_CLEAR = 4194316,
    KEY_HOME = 4194317,
    KEY_END = 4194318,
    KEY_LEFT = 4194319,
    KEY_UP = 4194320,
    KEY_RIGHT = 4194321,
    KEY_DOWN = 4194322,
    KEY_PAGEUP = 4194323,
    KEY_PAGEDOWN = 4194324,
    KEY_SHIFT = 4194325,
    KEY_CTRL = 4194326,
    KEY_META = 4194327,
    KEY_ALT = 4194328,
    KEY_CAPSLOCK = 4194329,
    KEY_NUMLOCK = 4194330,
    KEY_SCROLLLOCK = 4194331,
    KEY_F1 = 4194332,
    KEY_F2 = 4194333,
    KEY_F3 = 4194334,
    KEY_F4 = 4194335,
    KEY_F5 = 4194336,
    KEY_F6 = 4194337,
    KEY_F7 = 4194338,
    KEY_F8 = 4194339,
    KEY_F9 = 4194340,
    KEY_F10 = 4194341,
    KEY_F11 = 4194342,
    KEY_F12 = 4194343,
    KEY_F13 = 4194344,
    KEY_F14 = 4194345,
    KEY_F15 = 4194346,
    KEY_F16 = 4194347,
    KEY_F17 = 4194348,
    KEY_F18 = 4194349,
    KEY_F19 = 4194350,
    KEY_F20 = 4194351,
    KEY_F21 = 4194352,
    KEY_F22 = 4194353,
    KEY_F23 = 4194354,
    KEY_F24 = 4194355,
    KEY_F25 = 4194356,
    KEY_F26 = 4194357,
    KEY_F27 = 4194358,
    KEY_F28 = 4194359,
    KEY_F29 = 4194360,
    KEY_F30 = 4194361,
    KEY_F31 = 4194362,
    KEY_F32 = 4194363,
    KEY_F33 = 4194364,
    KEY_F34 = 4194365,
    KEY_F35 = 4194366,
    KEY_KP_MULTIPLY = 4194433,
    KEY_KP_DIVIDE = 4194434,
    KEY_KP_SUBTRACT = 4194435,
    KEY_KP_PERIOD = 4194436,
    KEY_KP_ADD = 4194437,
    KEY_KP_0 = 4194438,
    KEY_KP_1 = 4194439,
    KEY_KP_2 = 4194440,
    KEY_KP_3 = 4194441,
    KEY_KP_4 = 4194442,
    KEY_KP_5 = 4194443,
    KEY_KP_6 = 4194444,
    KEY_KP_7 = 4194445,
    KEY_KP_8 = 4194446,
    KEY_KP_9 = 4194447,
    KEY_MENU = 4194370,
    KEY_HYPER = 4194371,
    KEY_HELP = 4194373,
    KEY_BACK = 4194376,
    KEY_FORWARD = 4194377,
    KEY_STOP = 4194378,
    KEY_REFRESH = 4194379,
    KEY_VOLUMEDOWN = 4194380,
    KEY_VOLUMEMUTE = 4194381,
    KEY_VOLUMEUP = 4194382,
    KEY_MEDIAPLAY = 4194388,
    KEY_MEDIASTOP = 4194389,
    KEY_MEDIAPREVIOUS = 4194390,
    KEY_MEDIANEXT = 4194391,
    KEY_MEDIARECORD = 4194392,
    KEY_HOMEPAGE = 4194393,
    KEY_FAVORITES = 4194394,
    KEY_SEARCH = 4194395,
    KEY_STANDBY = 4194396,
    KEY_OPENURL = 4194397,
    KEY_LAUNCHMAIL = 4194398,
    KEY_LAUNCHMEDIA = 4194399,
    KEY_LAUNCH0 = 4194400,
    KEY_LAUNCH1 = 4194401,
    KEY_LAUNCH2 = 4194402,
    KEY_LAUNCH3 = 4194403,
    KEY_LAUNCH4 = 4194404,
    KEY_LAUNCH5 = 4194405,
    KEY_LAUNCH6 = 4194406,
    KEY_LAUNCH7 = 4194407,
    KEY_LAUNCH8 = 4194408,
    KEY_LAUNCH9 = 4194409,
    KEY_LAUNCHA = 4194410,
    KEY_LAUNCHB = 4194411,
    KEY_LAUNCHC = 4194412,
    KEY_LAUNCHD = 4194413,
    KEY_LAUNCHE = 4194414,
    KEY_LAUNCHF = 4194415,
    KEY_GLOBE = 4194416,
    KEY_KEYBOARD = 4194417,
    KEY_JIS_EISU = 4194418,
    KEY_JIS_KANA = 4194419,
    KEY_UNKNOWN = 8388607,
    KEY_SPACE = 32,
    KEY_EXCLAM = 33,
    KEY_QUOTEDBL = 34,
    KEY_NUMBERSIGN = 35,
    KEY_DOLLAR = 36,
    KEY_PERCENT = 37,
    KEY_AMPERSAND = 38,
    KEY_APOSTROPHE = 39,
    KEY_PARENLEFT = 40,
    KEY_PARENRIGHT = 41,
    KEY_ASTERISK = 42,
    KEY_PLUS = 43,
    KEY_COMMA = 44,
    KEY_MINUS = 45,
    KEY_PERIOD = 46,
    KEY_SLASH = 47,
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_COLON = 58,
    KEY_SEMICOLON = 59,
    KEY_LESS = 60,
    KEY_EQUAL = 61,
    KEY_GREATER = 62,
    KEY_QUESTION = 63,
    KEY_AT = 64,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_BRACKETLEFT = 91,
    KEY_BACKSLASH = 92,
    KEY_BRACKETRIGHT = 93,
    KEY_ASCIICIRCUM = 94,
    KEY_UNDERSCORE = 95,
    KEY_QUOTELEFT = 96,
    KEY_BRACELEFT = 123,
    KEY_BAR = 124,
    KEY_BRACERIGHT = 125,
    KEY_ASCIITILDE = 126,
    KEY_YEN = 165,
    KEY_SECTION = 167,
};

enum MouseButton {
    MOUSE_BUTTON_NONE = 0,
    MOUSE_BUTTON_LEFT = 1,
    MOUSE_BUTTON_RIGHT = 2,
    MOUSE_BUTTON_MIDDLE = 3,
    MOUSE_BUTTON_WHEEL_UP = 4,
    MOUSE_BUTTON_WHEEL_DOWN = 5,
    MOUSE_BUTTON_WHEEL_LEFT = 6,
    MOUSE_BUTTON_WHEEL_RIGHT = 7,
    MOUSE_BUTTON_XBUTTON1 = 8,
    MOUSE_BUTTON_XBUTTON2 = 9,
};


#endif
