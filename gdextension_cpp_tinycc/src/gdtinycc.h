#ifndef GDTINYCC_H
#define GDTINYCC_H


#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/canvas_layer.hpp> 
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <vector>
#include <string>


extern "C" {
    #include "tinycc-mob/libtcc.h"
}


typedef struct {
    int type;
    
    union {
        int i;
        float f;
        char s[256];
        int b;
        godot::Vector2 vec2;
        godot::Vector3 vec3;
        godot::Color color;
        godot::Rect2 rect2;
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

private:
    String source_file;
    String output_object_file;
    String input_object_file;

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
    void* get_tcc_state() const { return tcc_state; }
    int godot_input_event_is_pressed(void* evt);
    int godot_get_eventcode(void* event_ptr);

    void set_enable_2d_drawing(bool enabled);
    bool get_enable_2d_drawing() const { return enable_2d_drawing; }
    Node2D* get_drawingnode() const { return drawing_node; }
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