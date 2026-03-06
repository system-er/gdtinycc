
#include "gdtinycc_runtime.h"

void main() {
    godot_print("hello world from gdtinycc");
}

void _ready() {
    godot_print("C-_ready called!");
}