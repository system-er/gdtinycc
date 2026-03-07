
#include "stddef.h"
#include "gdtinycc_runtime.h"

void main() {
    godot_print("hello world from gdtinycc");
    void* testnode = godot_get_node("/root/Main");
    if (testnode != NULL) {
        godot_print("gdtinycc main: node found");
        GDExtensionVariant v = godot_get_variant(testnode, "name");
        godot_print(godot_get_type_name(v.type));
        godot_print(v.value.s);
    }
    else {
        godot_print("gdtinycc main: node not found");
    }

    void *label = godot_create("Label");
    if (!label) {
        godot_print("failed to create label");
        return;
    }
    GDExtensionVariant v;
    v.type = VARTYPE_STRING;
    snprintf(v.value.s, sizeof(v.value.s), "this is a label from gdtinycc");
    godot_set_variant(label, "text", v);

    GDExtensionVariant va;
    va.type = VARTYPE_VECTOR2;
    va.value.vec2.x = 200.0f;
    va.value.vec2.y = 100.0f;
    godot_set_variant(label, "position", va);

    void *main_node = godot_get_node("/root/Main");
    godot_add_child_deferred(main_node, label);

}

void _ready() {
    godot_print("gdtinycc-_ready called!");
}

void _process(double delta) {
}