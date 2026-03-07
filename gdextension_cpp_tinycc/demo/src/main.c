
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
}

void _ready() {
    godot_print("gdtinycc-_ready called!");
}

void _process(double delta) {
}