#ifndef GDTINYCC_H
#define GDTINYCC_H


#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/canvas_layer.hpp> 
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <godot_cpp/variant/vector3i.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/rect2i.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <vector>
#include <string>


extern "C" {
    #include "tinycc-mob/libtcc.h"
}


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

namespace godot {



class SignalHandler : public Node {
    GDCLASS(SignalHandler, Node)
public:
    void* callback_func = nullptr;
    void* user_data = nullptr;
    void _on_signal_callback() {
        if (callback_func) {
            using CallbackFunc = void(*)(void*);
            CallbackFunc func = (CallbackFunc)callback_func;
            func(user_data);
        }
    }
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("_on_signal_callback"), &SignalHandler::_on_signal_callback);
    }
};

class GDTinyCC : public Node {
    GDCLASS(GDTinyCC, Node)

public:
    String source_file;
    String output_object_file;
    String input_object_file;
    int compile_error_count = 0;
    int compile_warning_count = 0;
    String last_compile_error;
    String last_compile_warning;
    std::vector<String> compile_errors;
    std::vector<String> compile_warnings;
    String current_compile_file;

private:
    void *tcc_state;
    std::vector<SignalHandler*> signal_handlers;

    bool enable_2d_drawing = false;
    CanvasLayer* drawing_canvas = nullptr;
    Node2D* drawing_node = nullptr;


protected:
    static void _bind_methods();


public:

    GDTinyCC();
    ~GDTinyCC();

    void _ready();
    void _process(double delta);
    void _physics_process(double delta);
    void _input(const Ref<InputEvent> &event);
    void _unhandled_input(const Ref<InputEvent> &event);
    void _enter_tree();
    void _exit_tree();
    void _notification(int what);

    void compile_file();
    void compile_to_object(const String &output_file);
    void load_object(const String &object_file);
    void set_source_file(const String &p_path);
    String get_source_file() const;
    void set_output_object_file(const String &p_path);
    String get_output_object_file() const;
    void set_input_object_file(const String &p_path);
    String get_input_object_file() const;
    void load_object_file();
    bool connect_signal(void* node_ptr, const char* signal_name, void* callback_func, void* user_data);
    void disconnect_all_signals();
    int get_compile_error_count() const { return compile_error_count; }
    int get_compile_warning_count() const { return compile_warning_count; }
    String get_last_compile_error() const { return last_compile_error; }
    String get_last_compile_warning() const { return last_compile_warning; }
    Array get_compile_errors() const;
    Array get_compile_warnings() const;
    void clear_compile_messages();
    void* get_tcc_state() const { return tcc_state; }
    int godot_input_event_is_pressed(void* evt);
    int godot_get_eventcode(void* event_ptr);

    void set_enable_2d_drawing(bool enabled);
    bool get_enable_2d_drawing() const { return enable_2d_drawing; }
    Node2D* get_drawingnode() const { return drawing_node; }
    CanvasLayer* get_drawingcanvas() const { return drawing_canvas; }
};


class GDTinyCCDrawer : public Node2D {
    GDCLASS(GDTinyCCDrawer, Node2D)
public:
    GDTinyCC* parent_tcc = nullptr;

    void _draw() override {
        if (!parent_tcc || !parent_tcc->get_tcc_state()) return;
        //draw_circle(Vector2(400, 300), 100, Color(1, 0, 0)); //for test

        using DrawFunc = void (*)(void*, void*);
        auto draw_func = reinterpret_cast<DrawFunc>(
            tcc_get_symbol((TCCState*)parent_tcc->get_tcc_state(), "_draw")
        );

        if (draw_func) {
            draw_func(parent_tcc, this);
        }
    }

    static void _bind_methods() {}
};


}


#endif