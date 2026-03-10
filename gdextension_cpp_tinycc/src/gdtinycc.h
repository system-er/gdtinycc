#ifndef GDTINYCC_H
#define GDTINYCC_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <vector>
#include <string>

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
    static GDTinyCC* _current_instance;

    bool connect_signal(void* node_ptr, const char* signal_name, void* callback_func, void* user_data);
    void disconnect_all_signals();
};

}

#endif