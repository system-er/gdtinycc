
#include "stddef.h"
#include "gdtinycc_runtime.h"


void on_button_pressed(void* user_data, GDExtensionVariant arg) {
    godot_print("Button pressed!");
}

void main() {
    godot_print("hello world from GDTinyCC.");
    void* testnode = godot_get_node("/root/Main");
    if (testnode != NULL) {
        godot_print("GDTinyCC main: node found");
        GDExtensionVariant v = godot_get_variant(testnode, "name");
        godot_print(godot_get_type_name(v.type));
        godot_print(v.value.s);
    }
    else {
        godot_print("GDTinyCC main: node not found");
    }

    void *label = godot_create("Label");
    if (!label) {
        godot_print("failed to create label");
        return;
    }
    GDExtensionVariant v;
    v.type = VARTYPE_STRING;
    snprintf(v.value.s, sizeof(v.value.s), "this is a label from GDTinyCC");
    godot_set_variant(label, "text", v);

    GDExtensionVariant va;
    va.type = VARTYPE_VECTOR2;
    va.value.vec2.x = 200.0f;
    va.value.vec2.y = 20.0f;
    godot_set_variant(label, "position", va);

    void *main_node = godot_get_node("/root/Main");
    godot_add_child_deferred(main_node, label);

    void* button = godot_get_node("/root/Main/Button");
    godot_connect(button, "pressed", on_button_pressed, NULL);

}

void _ready() {
    godot_print("GDTinyCC _ready called!");
}

void _process(double delta) {
}

void _physics_process(double delta) {
}

void _input(void* event_ptr)
{
    //godot_print("input event!");
}
