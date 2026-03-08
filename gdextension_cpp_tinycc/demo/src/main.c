
#include "stddef.h"
#include "gdtinycc_runtime.h"


void on_button_pressed(void* user_data, GDExtensionVariant arg) {
    godot_print("Button pressed!");
}

void on_timeout(void* user_data) {
    godot_print("timeout!");
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

    void* timer = godot_create("Timer");
    GDExtensionVariant vt1;
    vt1.type = VARTYPE_FLOAT;
    vt1.value.f = 3.0f;
    godot_set_variant(timer, "wait_time", vt1);
    GDExtensionVariant vt2;
    vt2.type = VARTYPE_BOOL;
    vt2.value.f = 1;
    godot_set_variant(timer, "autostart", vt2);
    godot_connect(timer, "timeout", on_timeout, NULL);
    godot_add_child_deferred(testnode, timer);
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
