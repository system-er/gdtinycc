#ifndef GDTINYCC_H
#define GDTINYCC_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <vector>
#include <string>

struct SignalCallback {
    std::string signal_name;
    void* callback_func;
    void* user_data;
    godot::Object* source_node;
};

namespace godot {

class GDTinyCC : public Node {
    GDCLASS(GDTinyCC, Node)

private:
    String source_file;
    void *tcc_state;
    std::vector<SignalCallback> signal_callbacks;
    void* current_callback_func;
    void* current_user_data;

    void _on_signal_callback();

protected:
    static void _bind_methods();

public:
    GDTinyCC();
    ~GDTinyCC();

    void _ready();
    void _process(double delta);
    void _physics_process(double delta);
    void _input(const Ref<InputEvent> &event);

    void compile_file();
    void set_source_file(const String &p_path);
    String get_source_file() const;
    static GDTinyCC* _current_instance;

    bool connect_signal(void* node_ptr, const char* signal_name, void* callback_func, void* user_data);
    void disconnect_all_signals();
};

}

#endif